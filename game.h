#ifndef GAME_H_
#define GAME_H_

#include "player.h"

#include <SFML/Graphics.hpp>

#include <string_view>
#include <vector>

namespace rcg {

class Game {
 public:
  Game(std::string_view name);
  [[nodiscard]] bool Loaded() noexcept;
  void MainLoop();
 protected:
  void Raycast(sf::Image& buffer);
  void UpdateView(sf::Image& buffer);
  void UpdatePlayer(float frame_time);
 private:
  bool loaded_;
  sf::RenderWindow window_;
  std::vector<sf::Image> textures_;
  Player player_;
  std::vector<std::vector<int>> world_map_;
};

} // namespace rcg

#endif // GAME_H_