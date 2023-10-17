#ifndef GAME_H_
#define GAME_H_

#include "player.h"
#include "config.h"

#include <SFML/Graphics.hpp>

#include <array>
#include <vector>
#include <string_view>

namespace rcg {

enum class Cell : bool {
  kWall,
  kEmpty
};

class Game {
 public:
  Game(std::string_view name);
  void Loop();
 protected:
  void UpdateRays();
  void UpdatePlayer();
  void DrawMap();
  void DrawRays();
  void Draw3dSpace();
  void DrawMapCells();
  void DrawPlayer();
 private:
  Player player_;
  std::vector<std::vector<Cell>> map_;
  std::array<float, config::kWindowWidth> view_rays_;
  sf::RenderWindow window_;
  sf::Texture grid_texture_, wall_texture_, player_texture_, ig_wall_texture_;
  sf::Sprite grid_sprite_, wall_sprite_, player_sprite_, ig_wall_sprite_;
};


} // namespace rcg

#endif // GAME_H_