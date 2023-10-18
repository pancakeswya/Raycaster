#include "game.h"

#include <iostream>

int main() {
  rcg::Game game("Raycaster");
  if (!game.Loaded()) {
    std::cout << "Error: textures are not loaded" << std::endl;
  } else {
    game.MainLoop();
  }
  return 0;
}
