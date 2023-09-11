#include "raycast.h"
#include "util/point.h"

#include <algorithm>
#include <array>
#include <cmath>

namespace rcg {

Point<float> DistanceTowall(const Point<float>& player, float angle, const Map& map) {
  constexpr float raystep = 0.015f;
  float dx = raystep * std::cos(angle);
  float dy = raystep * std::sin(angle);
  float distance = 0.0f;
  float x,y;
  while (map.At(Point<float>(x,y)) != kWall) {
    distance += raystep;
    if (distance > 10000.0f) {
      return Point<float>(0.0f, 10000.0f);
    }
    x += dx;
    y += dy;
  }
  if (std::abs(x - std::round(x)) >
      std::abs(y - std::round(y))) {
    return Point<float>(distance, std::modf(y, &dy));
  }
  return Point<float>(distance, std::modf(x, &dx));
}

void drawScreen(SDL_Texture *screen, SDL_Surface* wall_img_surf, const Player& player, const Map& map, int height, int width) {
  void* pixels = nullptr;
  int pitch;
  SDL_LockTexture(screen, NULL, &pixels, &pitch);
  for (int y = 0; y < height / 2; y++) {
    Uint8 color = 255.0f - 255.0f * (float(y)/float(height));
    Uint32 ceilColor = ((0 + color) << 8) + 255;
    Uint32 floorColor = ((0 + color) << 16) + 255;
    for (int x = 0; x < width; x++) {
      *(((Uint32*)pixels) + width * y + x) = ceilColor;
      *(((Uint32 *)pixels) + width * (height - 1) - y * width + x) = floorColor;
    }
  }
  for (int x = 0; x < width; x++) {
    float rayAngle = std::atan((x - width / 2.0f) / (width / 2.0f));
    float castAngle = player.A() + rayAngle;
    auto rayReturn = DistanceTowall(player, castAngle, map);
    double distance = std::cos(rayAngle) * rayReturn.X();

    Uint8 color = 255.0f * exp(-distance/ 20.0f);

    int wallHeight = 1.0f / distance * height;
    int actualWallHeight = wallHeight;
    wallHeight = (wallHeight > height) ? height : wallHeight;
    int wally = (int)((height - wallHeight) / 2);
    double wallcoord = rayReturn.Y();
    for (int y = wally; y < wallHeight + wally; y++) {
      // Get where on the texture we are.
      int texture_x = 0;
      int texture_y = 0;
      if (distance > 1.0f) {
        texture_x = wallcoord * 127;
        texture_y = (y - wally) / (float)wallHeight * 127;
      } else {
        texture_x = wallcoord * 127;
        texture_y = ((actualWallHeight - wallHeight) / 2.0f + y) / (float)actualWallHeight * 127;
      }
      Uint8 r = *(((Uint8*)(wall_img_surf->pixels)) + texture_y * 3 * 128 + 3 * texture_x + 0);
      Uint8 g = *(((Uint8*)(wall_img_surf->pixels)) + texture_y * 3 * 128 + 3 * texture_x + 1);
      Uint8 b = *(((Uint8*)(wall_img_surf->pixels)) + texture_y * 3 * 128 + 3 * texture_x + 2);
      r = color / 255.0;
      g = color / 255.0;
      b = color / 255.0;
      Uint32 wallColor = 0;
      wallColor = (((((b << 8) + g) << 8) + r) << 8) + 255;
      *(((Uint32*)pixels) + width * y + x) = wallColor;
    }
  }
  SDL_UnlockTexture(screen);
}

  ColorMap RayCast(const Player& player, const Map& map, int width, int height, float depth) {
    ColorMap view;
    view.reserve(width * height);
    for (int x{}; x < width; ++x) {
      float ray_angle = (player.A() - player.Fov() / 2.0f) +
                        (float(x) / width) * player.Fov();

      float step_size{0.01f};
      float distance_to_wall{};

      bool hits_wall{};
      bool is_boundary{};

      Point<float> eye(std::sin(ray_angle), std::cos(ray_angle));

      while (!hits_wall && distance_to_wall < depth) {
        distance_to_wall += step_size;
        Point<int> ray(player.X() + eye.X() * distance_to_wall,
                       player.Y() + eye.Y() * distance_to_wall);

        if (ray.X() < 0 || ray.X() >= map.Width() ||
            ray.Y() < 0 || ray.Y() >= map.Height()) {
          hits_wall = true;
          distance_to_wall = depth;
        } else {
          if (map.At(ray) == kWall) {
            hits_wall = true;
            std::array<std::pair<float, float>, 4> p;
            for (int tx{}; tx < 2; ++tx) {
              for (int ty{}; ty < 2; ++ty) {
                auto vx = float(ray.X() + tx) - player.X();
                auto vy = float(ray.Y() + ty) - player.Y();
                float d = std::sqrt(vx * vx + vy * vy);
                float dot = (eye.X() * vx / d) + (eye.Y() * vy / d);
                p[ty * 2 + tx] = {d, dot};
              }
            }
            std::sort(p.begin(), p.end());
            if (std::acos(p[0].second) < 0.01f ||
                std::acos(p[1].second) < 0.01f ||
                std::acos(p[2].second) < 0.01f) {
              is_boundary = true;
            }
          }
        }
      }
      int ceiling = (height / 2.0f) - height / (distance_to_wall);
      int floor = height - ceiling;
      SDL::Color color;
      if (distance_to_wall <= depth / 4.0f) {
        color = SDL::LightGray;
      } else if (distance_to_wall < depth / 3.0f) {
        color = SDL::LightGray;
      } else if (distance_to_wall < depth / 2.0f) {
        color = SDL::LightGray;
      } else if (distance_to_wall < depth) {
        color = SDL::LightGray;
      } else {
        color = SDL::Color{165, 165, 165, 255};
      }
      if (is_boundary) {
        color = SDL::Black;
      }
      for (int y{}; y < height; ++y) {
        SDL::Color color_to_draw = color;
        if (y <= ceiling) {
          color_to_draw = SDL::White;
        } else if (y < ceiling || y > floor) {
          float b = 1.0f - ((float(y) - height / 2.0f) / (height / 2.0f));
          if (b < 0.25f) {
            color_to_draw = SDL::DarkGray;
          } else if (b < 0.5f) {
            color_to_draw = SDL::DarkGray;
          } else if (b < 0.75f) {
            color_to_draw = SDL::DarkGray;
          } else if (b < 0.9f) {
            color_to_draw = SDL::DarkGray;
          } else {
            color_to_draw = SDL::White;
          }
        }
        view[y * width + x] = color_to_draw;
      }
    }
    return view;
    }

  }  // namespace rcg