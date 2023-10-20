#include "game.h"
#include "config.h"

#include <array>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace rcg {

namespace {

std::vector<std::vector<int>> LoadMap() {
  std::vector<std::vector<int>> map(config::kMapWidth, std::vector<int>(config::kMapHeight));
  for(int y = 0; y < config::kMapWidth; ++y) {
    for(int x = 0; x < config::kMapHeight; ++x) {
      map[y][x] = config::kWorldMap[y][x];
    }
  }
  return map;
}

inline sf::Color DarkerColor(sf::Color color) noexcept {
  color.r /= config::kShadingCoef;
  color.g /= config::kShadingCoef;
  color.b /= config::kShadingCoef;
  return color;
}

void SortSprites(std::array<int, config::kSpriteCount>& order, std::array<double, config::kSpriteCount>& dist) {
  std::vector<std::pair<double, int>> sprites(config::kSpriteCount);
  for(int i = 0; i < config::kSpriteCount; ++i) {
    sprites[i].first = dist[i];
    sprites[i].second = order[i];
  }
  std::sort(sprites.begin(), sprites.end());
  for(int i = 0; i < config::kSpriteCount; i++) {
    dist[i] = sprites[config::kSpriteCount - i - 1].first;
    order[i] = sprites[config::kSpriteCount - i - 1].second;
  }
}

} // namespace

Game::Game(std::string_view name)
    : player_(config::kStartX, config::kStartY,
              config::kStartDirX, config::kStartDirY,
              config::kStartPlaneX, config::kStartPlaneY),
      loaded_(true),
      window_(sf::VideoMode(config::kWindowWidth, config::kWindowHeight), name.data(), sf::Style::Close),
      textures_(config::kTextureCount),
      pistol_textures_(config::kPistolTextureCount),
      world_map_(LoadMap()) {
  window_.setSize(sf::Vector2u(config::kResizeWindow * config::kWindowWidth,
                               config::kResizeWindow * config::kWindowHeight));
  window_.setFramerateLimit(config::kFps);
  LoadTextures();
}

bool Game::Loaded() noexcept {
  return loaded_;
}

inline void Game::LoadTextures() {
  loaded_ &= textures_[0].loadFromFile("pics/eagle.png");
  loaded_ &= textures_[1].loadFromFile("pics/redbrick.png");
  loaded_ &= textures_[2].loadFromFile("pics/purplestone.png");
  loaded_ &= textures_[3].loadFromFile("pics/greystone.png");
  loaded_ &= textures_[4].loadFromFile("pics/bluestone.png");
  loaded_ &= textures_[5].loadFromFile("pics/mossy.png");
  loaded_ &= textures_[6].loadFromFile("pics/wood.png");
  loaded_ &= textures_[7].loadFromFile("pics/colorstone.png");

  loaded_ &= textures_[8].loadFromFile("pics/barrel.png");
  loaded_ &= textures_[9].loadFromFile("pics/pillar.png");
  loaded_ &= textures_[10].loadFromFile("pics/greenlight.png");

  loaded_ &= pistol_textures_[0].loadFromFile("pics/pistol0.png");
  loaded_ &= pistol_textures_[1].loadFromFile("pics/pistol1.png");
  loaded_ &= pistol_textures_[2].loadFromFile("pics/pistol2.png");
  loaded_ &= pistol_textures_[3].loadFromFile("pics/pistol3.png");
  loaded_ &= pistol_textures_[4].loadFromFile("pics/pistol4.png");
}

struct Sprite
{
  double x;
  double y;
  int texture;
};

Sprite sprite[config::kSpriteCount] = {
        {20.5, 11.5, 10},
        {18.5,4.5, 10},
        {10.0,4.5, 10},
        {10.0,12.5,10},
        {3.5, 6.5, 10},
        {3.5, 20.5,10},
        {3.5, 14.5,10},
        {14.5,20.5,10},

        {18.5, 10.5, 9},
        {18.5, 11.5, 9},
        {18.5, 12.5, 9},

        {21.5, 1.5, 8},
        {15.5, 1.5, 8},
        {16.0, 1.8, 8},
        {16.2, 1.2, 8},
        {3.5,  2.5, 8},
        {9.5, 15.5, 8},
        {10.0, 15.1,8},
        {10.5, 15.8,8},
};

void Game::Raycast(sf::Image& buffer) {
  //parameters for scaling and moving the sprites
  constexpr int u_div = 1;
  constexpr int v_div  = 1;
  constexpr float v_move = 0.0f;
  std::array<double, config::kWindowWidth> z_buffer;
  std::array<int,config::kSpriteCount> sprite_order;
  std::array<double, config::kSpriteCount> sprite_distance;
  // cast a floor
  for (int y = config::kWindowHeight / 2 + 1; y < config::kWindowHeight; ++y) {
    float ray_dir_x0 = player_.DirX() - player_.PlaneX();
    float ray_dir_y0 = player_.DirY() - player_.PlaneY();
    float ray_dir_x1 = player_.DirX() + player_.PlaneX();
    float ray_dir_y1 = player_.DirY() + player_.PlaneY();

    int p = y - config::kWindowHeight / 2;
    float pos_z = 0.5f * config::kWindowHeight;
    float row_distance = pos_z / p;

    float floor_step_x = row_distance * (ray_dir_x1 - ray_dir_x0) / config::kWindowWidth;
    float floor_step_y = row_distance * (ray_dir_y1 - ray_dir_y0) / config::kWindowWidth;

    float floor_x = player_.X() + row_distance * ray_dir_x0;
    float floor_y = player_.Y() + row_distance * ray_dir_y0;

    for (int x = 0; x < config::kWindowWidth; ++x) {
      auto cell_x = static_cast<int>(floor_x);
      auto cell_y = static_cast<int>(floor_y);

      auto tx = static_cast<int>(config::kTexWidth * (floor_x - cell_x)) & (config::kTexWidth - 1);
      auto ty = static_cast<int>(config::kTexHeight * (floor_y - cell_y)) & (config::kTexHeight - 1);

      floor_x += floor_step_x;
      floor_y += floor_step_y;

      sf::Color color = textures_[config::kFloorTexture].getPixel(tx, ty);

      buffer.setPixel(x, y, DarkerColor(color));

      color = textures_[config::kCeilTexture].getPixel(tx, ty);

      buffer.setPixel(x, config::kWindowHeight - y - 1, color);
    }
  }
  // cast a wall
  for (int x = 0; x < config::kWindowWidth; ++x) {
    float camera_x = 2.0f * x / static_cast<float>(config::kWindowWidth) - 1;
    float ray_dir_x = player_.DirX() + player_.PlaneX() * camera_x;
    float ray_dir_y = player_.DirY() + player_.PlaneY() * camera_x;

    auto map_x = static_cast<int>(player_.X());
    auto map_y = static_cast<int>(player_.Y());

    float side_dist_x, side_dist_y;

    float delta_dist_x = (ray_dir_x == 0.0f) ? 1e30f : std::abs(1.0f / ray_dir_x);
    float delta_dist_y = (ray_dir_y == 0.0f) ? 1e30f : std::abs(1.0f / ray_dir_y);
    float perp_wall_dist;

    int step_x, step_y;

    bool side;

    if (ray_dir_x < 0) {
      step_x = -1;
      side_dist_x = (player_.X() - map_x) * delta_dist_x;
    } else {
      step_x = 1;
      side_dist_x = (map_x + 1.0f - player_.X()) * delta_dist_x;
    }

    if (ray_dir_y < 0) {
      step_y = -1;
      side_dist_y = (player_.Y() - map_y) * delta_dist_y;
    } else {
      step_y = 1;
      side_dist_y = (map_y + 1.0f - player_.Y()) * delta_dist_y;
    }

    do {
      if (side_dist_x < side_dist_y) {
        side_dist_x += delta_dist_x;
        map_x += step_x;
        side = false;
      } else {
        side_dist_y += delta_dist_y;
        map_y += step_y;
        side = true;
      }
    } while (!world_map_[map_x][map_y]);

    if (!side) {
      perp_wall_dist = (side_dist_x - delta_dist_x);
    } else {
      perp_wall_dist = (side_dist_y - delta_dist_y);
    }

    int line_height = static_cast<int>(config::kWindowHeight / perp_wall_dist);

    int draw_start = -line_height / 2 + config::kWindowHeight / 2;
    if (draw_start < 0) {
      draw_start = 0;
    }
    int draw_end = line_height / 2 + config::kWindowHeight / 2;
    if (draw_end >= config::kWindowHeight) {
      draw_end = config::kWindowHeight - 1;
    }

    int tex_num = world_map_[map_x][map_y] - 1;

    float wall_x;
    if (!side) {
      wall_x = player_.Y() + perp_wall_dist * ray_dir_y;
    } else {
      wall_x = player_.X() + perp_wall_dist * ray_dir_x;
    }

    wall_x -= std::floor(wall_x);

    auto tex_x = static_cast<long long int>(wall_x * static_cast<float>(config::kTexWidth));
    if (!side && ray_dir_x > 0.0f) {
      tex_x = config::kTexWidth - tex_x - 1;
    }
    if (side && ray_dir_y < 0.0f) {
      tex_x = config::kTexWidth - tex_x - 1;
    }
    float step = 1.0f * config::kTexHeight / line_height;

    float tex_pos = (draw_start - config::kWindowHeight / 2 + line_height / 2) * step;
    for (int y = draw_start; y < draw_end; ++y) {
      long long int tex_y = static_cast<int>(tex_pos) & (config::kTexHeight - 1);
      tex_pos += step;
      sf::Color color = textures_[tex_num].getPixel(tex_x, tex_y);
      if (side) {
        color = DarkerColor(color);
      }
      buffer.setPixel(x, y, color);
    }
    z_buffer[x] = perp_wall_dist;
  }
// Cast sprites
  for(int i = 0; i < config::kSpriteCount; ++i) {
    sprite_order[i] = i;
    sprite_distance[i] = ((player_.X() - sprite[i].x) * (player_.X() - sprite[i].x) + (player_.Y() - sprite[i].y) * (player_.Y() - sprite[i].y));
  }
  SortSprites(sprite_order, sprite_distance);

  for(int order : sprite_order) {
    double sprite_x = sprite[order].x - player_.X();
    double sprite_y = sprite[order].y - player_.Y();

    double inv_det = 1.0 / (player_.PlaneX() * player_.DirY() - player_.DirX() * player_.PlaneY());

    double transform_x = inv_det * (player_.DirY() * sprite_x - player_.DirX() * sprite_y);
    double transform_y = inv_det * (-player_.PlaneY() * sprite_x + player_.PlaneX() * sprite_y); //this is actually the depth inside the screen, that what Z is in 3D, the distance of sprite to player, matching sqrt(spriteDistance[i])

    auto sprite_screen_x = static_cast<int>((config::kWindowWidth / 2) * (1 + transform_x / transform_y));

    auto v_move_screen = static_cast<int>(v_move / transform_y);

    int sprite_height = std::abs(static_cast<int>(config::kWindowHeight / (transform_y))) / v_div; //using "transformY" instead of the real distance prevents fisheye

    int draw_start_y = -sprite_height / 2 + config::kWindowHeight / 2 + v_move_screen;
    if (draw_start_y < 0) {
      draw_start_y = 0;
    }
    int draw_end_y = sprite_height / 2 + config::kWindowHeight / 2 + v_move_screen;
    if (draw_end_y >= config::kWindowHeight) {
      draw_end_y = config::kWindowHeight - 1;
    }

    int sprite_width = std::abs(static_cast<int>(config::kWindowHeight / (transform_y))) / u_div;
    int draw_start_x = -sprite_width / 2 + sprite_screen_x;
    if (draw_start_x < 0) {
      draw_start_x = 0;
    }
    int draw_end_x = sprite_width / 2 + sprite_screen_x;
    if (draw_end_x > config::kWindowWidth) {
      draw_end_x = config::kWindowWidth;
    }
    for (int stripe = draw_start_x; stripe < draw_end_x; ++stripe) {
      long long int tex_x = static_cast<long long int>(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * config::kTexWidth / sprite_width) / 256;

      if(transform_y > 0 && transform_y < z_buffer[stripe]) {
        for(int y = draw_start_y; y < draw_end_y; ++y) {
          long long int d = (y - v_move_screen) * 256 - config::kWindowHeight * 128 + sprite_height * 128;
          long long int tex_y = ((d * config::kTexHeight) / sprite_height) / 256;
          sf::Color color = textures_[sprite[order].texture].getPixel(tex_x, tex_y);
          if (color != sf::Color::Black) {
            buffer.setPixel(stripe, y, DarkerColor(color));
          }
        }
      }
    }
  }

}

void Game::UpdateView(sf::Image& buffer) {
  sf::Texture texture;
  texture.create(config::kWindowWidth, config::kWindowHeight);
  texture.loadFromImage(buffer);
  sf::Sprite sprite(texture);

  window_.draw(sprite);
}

void Game::UpdatePlayer(float frame_time) {
  float step = frame_time * config::kMoveSpeed;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    player_.RotateX(frame_time * config::kRotationSpeed);
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
    player_.RotateX(frame_time * -config::kRotationSpeed);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    if (!world_map_[int(player_.X() - player_.DirX() * step)][int(player_.Y())]) {
      player_.MoveBackwardX(step);
    }
    if (!world_map_[int(player_.X())][int(player_.Y()  - player_.DirY() * step)]) {
      player_.MoveBackwardY(step);
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    if (!world_map_[int(player_.X() + player_.DirX() * step)][int(player_.Y())]) {
      player_.MoveForwardX(step);
    }
    if (!world_map_[int(player_.X())][int(player_.Y()  + player_.DirY() * step)]) {
      player_.MoveForwardY(step);
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    if (!world_map_[int(player_.X() - player_.PlaneX() * step)][int(player_.Y())]) {
      player_.MoveLeftX(step);
    }
    if (!world_map_[int(player_.X())][int(player_.Y()  - player_.PlaneY() * step)]) {
      player_.MoveLeftY(step);
    }
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    if (!world_map_[int(player_.X() + player_.PlaneX() * step)][int(player_.Y())]) {
      player_.MoveRightX(step);
    }
    if (!world_map_[int(player_.X())][int(player_.Y()  + player_.PlaneY() * step)]) {
      player_.MoveRightY(step);
    }
  }
}

void Game::MainLoop() {
  sf::Clock clock_frames, clock_pistol;
  sf::Image buffer;
  buffer.create(config::kWindowWidth, config::kWindowHeight);

  sf::Sprite pistol_sprite;
  {
    // init pistol sprite
    pistol_sprite.setTexture(pistol_textures_[0]);
    float c = config::kWindowHeight / pistol_sprite.getLocalBounds().height;
    pistol_sprite.setScale(c, c);
    float off = (config::kWindowWidth - pistol_sprite.getLocalBounds().width * c) * 0.5f;
    pistol_sprite.setPosition(off, 0.0f);
  }
  bool shot = false;
  int pistol_tex_num = 1;

  while (window_.isOpen()) {
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window_.close();
      } else if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        shot = true;
      }
    }
    auto pistol_ticks = clock_pistol.getElapsedTime();
    if (shot && pistol_ticks.asMicroseconds() > config::kPistolAnimationDuration) {
      // play pistol animation
      if (pistol_tex_num == 5) {
        shot = false;
        pistol_tex_num = 0;
      }
      pistol_sprite.setTexture(pistol_textures_[pistol_tex_num++]);
      clock_pistol.restart();
    }

    window_.clear();

    Raycast(buffer);
    UpdateView(buffer);

    auto fps = clock_frames.getElapsedTime();
    clock_frames.restart();

    UpdatePlayer(fps.asSeconds());
    window_.draw(pistol_sprite);
    window_.display();
  }
}

} // namespace rcg