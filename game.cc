#include "game.h"
#include "degreem.h"

#include <chrono>
#include <iostream>
#include <limits>

namespace rcg {

namespace {

std::vector<std::vector<Cell>> LoadMap(std::string_view map_path) {
  sf::Image map_sketch;
  map_sketch.loadFromFile(map_path.data());

  std::vector<std::vector<Cell>> map(config::kMapWidth, std::vector<Cell>(config::kMapHeight,Cell::kEmpty));

  for(int i = 0; i < config::kMapWidth; ++i) {
    for(int j = 0; j < config::kMapHeight; ++j) {
      sf::Color pixel = map_sketch.getPixel(i, j);
      if (pixel == sf::Color::Black) {
        map[i][j] = Cell::kWall;
      }
    }
  }
  return map;
}

bool CollidesWithMap(float x, float y, const std::vector<std::vector<Cell>>& map) {
  float cell_x = x / config::kCellSize;
  float cell_y = y / config::kCellSize;
  int curr_x = int(std::floor(cell_x));
  int curr_y = int(std::floor(cell_y));
  int next_x = int(std::ceil(cell_x));
  int next_y = int(std::ceil(cell_y));
  return map[curr_x][curr_y] == Cell::kWall || map[next_x][curr_y] == Cell::kWall ||
      map[curr_x][next_y] == Cell::kWall || map[next_x][next_y] == Cell::kWall;
}

void HandlePlayerKeys(Player& player) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    player.RotateLeft();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    player.RotateRight();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    player.MoveLeft();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    player.MoveRight();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    player.MoveBackward();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    player.MoveForward();
  }
}


} // namespace


Game::Game(std::string_view name)
  : player_(config::kPlayerStartX,config::kPlayerStartY), map_(LoadMap(config::kMapPath.data())),
    window_(sf::VideoMode(config::kWindowWidth, config::kWindowHeight), name.data(), sf::Style::Close) {

  window_.setView(sf::View(sf::FloatRect(0, 0, config::kWindowWidth, config::kWindowHeight)));

  grid_texture_.loadFromFile(config::kMapGridCellPath.data());
  wall_texture_.loadFromFile(config::kMapWallPath.data());
  player_texture_.loadFromFile(config::kMapPlayerPath.data());
  ig_wall_texture_.loadFromFile(config::kIgWallPath.data());

  wall_sprite_.setTexture(wall_texture_);
  grid_sprite_.setTexture(grid_texture_);
  player_sprite_.setTexture(player_texture_);
  ig_wall_sprite_.setTexture(ig_wall_texture_);

  grid_sprite_.setTextureRect(sf::IntRect(0, 0, config::kMapGridCellSize, config::kMapGridCellSize));
}

void Game::UpdatePlayer() {
  Player next_player = player_;
  HandlePlayerKeys(next_player);
  if (!CollidesWithMap(next_player.X(), next_player.Y(), map_)) {
    std::swap(player_, next_player);
  } else if (!CollidesWithMap(next_player.X(), player_.Y(), map_)) {
    player_.SetX(next_player.X());
    player_.SetY(config::kCellSize * std::round(player_.Y() / config::kCellSize));
  } else if (!CollidesWithMap(player_.X(), next_player.Y(), map_)) {
    player_.SetX(config::kCellSize * std::round(player_.X() / config::kCellSize));
    player_.SetY(next_player.Y());
  } else {
    player_.SetX(config::kCellSize * std::round(player_.X() / config::kCellSize));
    player_.SetY(config::kCellSize * std::round(player_.Y() / config::kCellSize));
  }
}

void Game::UpdateRays() {
  for (int i = 0; i < config::kWindowWidth; ++i) {
    int cell_step_x = 0;
    int cell_step_y = 0;

    float ray_direction = dgm::ToValidDegrees(
        player_.Pov() + config::kFov * (std::floor(0.5f * config::kWindowWidth) - i) / (config::kWindowWidth - 1));
    float ray_direction_x = std::cos(dgm::DegreeToRadians(ray_direction));
    float ray_direction_y = -std::sin(dgm::DegreeToRadians(ray_direction));

    float ray_length = 0;
    float ray_start_x = player_.X() + 0.5f * config::kCellSize;
    float ray_start_y = player_.Y() + 0.5f * config::kCellSize;

    float x_ray_length = 0;
    float y_ray_length = 0;

    auto
        x_ray_unit_length = float(config::kCellSize * std::sqrt(1 + std::pow(ray_direction_y / ray_direction_x, 2.0f)));
    auto
        y_ray_unit_length = float(config::kCellSize * std::sqrt(1 + std::pow(ray_direction_x / ray_direction_y, 2.0f)));

    auto current_cell_x = int(std::floor(ray_start_x / config::kCellSize));
    auto current_cell_y = int(std::floor(ray_start_y / config::kCellSize));

    if (ray_direction_x < 0) {
      cell_step_x = -1;
      x_ray_length = x_ray_unit_length * (ray_start_x / config::kCellSize - current_cell_x);
    } else if (ray_direction_x > 0) {
      cell_step_x = 1;
      x_ray_length = x_ray_unit_length * (1 + current_cell_x - ray_start_x / config::kCellSize);
    }

    if (ray_direction_y < 0) {
      cell_step_y = -1;
      y_ray_length = y_ray_unit_length * (ray_start_y / config::kCellSize - current_cell_y);
    } else if (ray_direction_y > 0) {
      cell_step_y = 1;
      y_ray_length = y_ray_unit_length * (1 + current_cell_y - ray_start_y / config::kCellSize);
    }

    while (ray_length <= config::kRenderDistance) {
      bool corner_collision = false;

      if (x_ray_length < y_ray_length) {
        ray_length = x_ray_length;
        x_ray_length += x_ray_unit_length;
        current_cell_x += cell_step_x;
      } else if (x_ray_length > y_ray_length) {
        ray_length = y_ray_length;
        y_ray_length += y_ray_unit_length;
        current_cell_y += cell_step_y;
      } else {
        corner_collision = true;

        ray_length = x_ray_length;
        x_ray_length += x_ray_unit_length;
        y_ray_length += y_ray_unit_length;

        current_cell_x += cell_step_x;
        current_cell_y += cell_step_y;
      }

      if (current_cell_x >= 0 && current_cell_y >= 0 &&
          current_cell_y < config::kMapHeight && current_cell_x < config::kMapWidth &&
          (map_[current_cell_x][current_cell_y] == Cell::kWall ||
              corner_collision &&
                  map_[current_cell_x - cell_step_x][current_cell_y] == Cell::kWall &&
                  map_[current_cell_x][current_cell_y - cell_step_y] == Cell::kWall)) {
        break;
      }
    }

    ray_length = std::min(config::kRenderDistance, ray_length);
    view_rays_[i] = ray_length;
  }
}

void Game::DrawMapCells() {
  int scaled_w = std::ceil(config::kMapCellSize * config::kMapWidth / float(config::kMapGridCellSize));
  int scaled_h = std::ceil(config::kMapCellSize * config::kMapHeight / float(config::kMapGridCellSize));

  for(unsigned int i = 0; i < scaled_w; ++i) {
    for(unsigned int j = 0; j < scaled_h; ++j) {
      grid_sprite_.setPosition(float(config::kMapGridCellSize * i), float(config::kMapGridCellSize * j));
      window_.draw(grid_sprite_);
    }
  }
}

void Game::DrawMap() {
  for(int i = 0; i < config::kMapWidth; ++i) {
    for(int j = 0; j < config::kMapHeight; ++j) {
      if (map_[i][j] == Cell::kWall) {
        wall_sprite_.setPosition(float(config::kMapCellSize * i), float(config::kMapCellSize * j));
        window_.draw(wall_sprite_);
      }
    }
  }
}

void Game::DrawPlayer() {
  float frame_angle = 360.f * config::kMapCellSize / player_texture_.getSize().x;
  float shifted_direction = dgm::ToValidDegrees(player_.Pov() + 0.5f * frame_angle);

  player_sprite_.setPosition(std::round(config::kMapCellSize * player_.X() / config::kCellSize), std::round(config::kMapCellSize * player_.Y() / config::kCellSize));
  player_sprite_.setTextureRect(sf::IntRect(int(config::kMapCellSize * std::floor(shifted_direction / frame_angle)), 0, config::kMapCellSize, config::kMapCellSize));

  window_.draw(player_sprite_);
}

void Game::DrawRays() {
  float start_x = player_.X() + 0.5f * config::kCellSize;
  float start_y = player_.Y() + 0.5f * config::kCellSize;

  sf::VertexArray fov_visualization(sf::TriangleFan, config::kWindowWidth + 1);
  fov_visualization[0].position = sf::Vector2f(config::kMapCellSize * start_x / config::kCellSize, config::kMapCellSize * start_y / config::kCellSize);
  for (int i = 0; i < config::kWindowWidth; ++i) {
    float ray_direction = dgm::ToValidDegrees(player_.Pov() + config::kFov * (std::floor(0.5f * config::kWindowWidth) - i - 1) / (config::kWindowWidth - 1));
    fov_visualization[i + 1].position = sf::Vector2f(config::kMapCellSize * (start_x + view_rays_[i] * std::cos(dgm::DegreeToRadians(ray_direction))) / config::kCellSize,
                                                     config::kMapCellSize * (start_y - view_rays_[i] * std::sin(dgm::DegreeToRadians(ray_direction))) / config::kCellSize);
  }
  window_.draw(fov_visualization);
}

void Game::Draw3dSpace() {
  float projection_distance =  0.5f * config::kCellSize / std::tan(dgm::DegreeToRadians(0.5f * 58.75f));
  float ray_start_x = player_.X() + 0.5f * config::kCellSize;
  float ray_start_y = player_.Y() + 0.5f * config::kCellSize;

  int previous_column = std::numeric_limits<int>::min();

  sf::RectangleShape floor_shape(sf::Vector2f(config::kWindowWidth, 0.5f * config::kWindowHeight));
  floor_shape.setFillColor(sf::Color(36, 219, 0));
  floor_shape.setPosition(0.0f, 0.5f * config::kWindowHeight);

  window_.draw(floor_shape);

  for(int i = 0; i < config::kWindowWidth; ++i) {
      float ray_direction = config::kFov * (std::floor(0.5f * config::kWindowWidth) - i) / (config::kWindowWidth - 1);
      float ray_projection_position = 0.5f * std::tan(dgm::DegreeToRadians(ray_direction)) / std::tan(dgm::DegreeToRadians(0.5f * config::kFov));

      auto current_column = int(std::round(config::kWindowWidth * (0.5f - ray_projection_position)));
      auto next_column = config::kWindowWidth;

      if (i < config::kWindowWidth - 1) {
        float next_ray_direction = config::kFov * (std::floor(0.5f * config::kWindowWidth) - i - 1) / (config::kWindowWidth - 1);

        ray_projection_position = 0.5f * std::tan(dgm::DegreeToRadians(next_ray_direction)) / std::tan(dgm::DegreeToRadians(0.5f * config::kFov));

        next_column = int(std::round(config::kWindowWidth * (0.5f - ray_projection_position)));
      }

      if (previous_column < current_column) {
        float ray_end_x = ray_start_x + view_rays_[i] * std::cos(dgm::DegreeToRadians(dgm::ToValidDegrees(player_.Pov() + ray_direction)));
        float ray_end_y = ray_start_y - view_rays_[i] * std::sin(dgm::DegreeToRadians(dgm::ToValidDegrees(player_.Pov() + ray_direction)));
        float wall_texture_column_x;

        auto brightness = int(std::round(255 * std::max<float>(0, 2 * view_rays_[i] / config::kRenderDistance - 1)));
        auto column_height = int(config::kWindowHeight * projection_distance / (view_rays_[i] * std::cos(dgm::DegreeToRadians(ray_direction))));

        sf::RectangleShape shape(sf::Vector2f(std::max(1, next_column - current_column), column_height));
        shape.setFillColor(sf::Color(73, 255, 255, brightness));
        shape.setPosition(current_column, std::round(0.5f * (config::kWindowHeight - column_height)));

        previous_column = current_column;

        if (std::abs(ray_end_x - config::kCellSize * std::round(ray_end_x / config::kCellSize)) <
            std::abs(ray_end_y - config::kCellSize * std::round(ray_end_y / config::kCellSize))) {
          wall_texture_column_x = ray_end_y - config::kCellSize * std::floor(ray_end_y / config::kCellSize);
        } else {
          wall_texture_column_x = config::kCellSize * std::ceil(ray_end_x / config::kCellSize) - ray_end_x;
        }

        ig_wall_sprite_.setPosition(current_column, std::round(0.5f * (config::kWindowHeight - column_height)));
        ig_wall_sprite_.setTextureRect(sf::IntRect(int(std::round(wall_texture_column_x)), 0, 1, config::kCellSize));
        ig_wall_sprite_.setScale(std::max(1, next_column - current_column), column_height / float(config::kCellSize));

        window_.draw(ig_wall_sprite_);
        window_.draw(shape);
      }
  }
}


void Game::Loop() {
  sf::Event event;
  auto previous_time = std::chrono::steady_clock::now();
  std::chrono::microseconds lag(0);

  while(window_.isOpen()) {
    auto delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);
    lag += delta_time;
    previous_time += delta_time;
    while (lag >= rcg::config::kFrameDuration) {
      lag -= rcg::config::kFrameDuration;
      while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window_.close();
          break;
        }
      }
      UpdatePlayer();
      UpdateRays();
      if (lag < rcg::config::kFrameDuration) {
        window_.clear(sf::Color::Blue);
        Draw3dSpace();
        DrawMapCells();
        DrawMap();
        DrawPlayer();
        DrawRays();
      }
      window_.display();
    }
  }
}


} // namespace rcg