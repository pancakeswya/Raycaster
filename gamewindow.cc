#include "gamewindow.h"

#include <chrono>

namespace rcg {

void GameWindow::HandleInput(SDL::Event &event, float elapsed_time) {
  auto keys = event.GetKeyboardState();
  auto &[player, map] = settings_;
  if (keys[SDL::Key::kUp]) {
    player.MoveUp(elapsed_time);
    if (map.At(player) == kWall) {
      player.MoveDown(elapsed_time);
    }
  }
  if (keys[SDL::Key::kDown]) {
    player.MoveDown(elapsed_time);
    if (map.At(player) == kWall) {
      player.MoveUp(elapsed_time);
    }
  }
  if (keys[SDL::Key::kLeft]) {
    player.RotateLeft(elapsed_time);
  }
  if (keys[SDL::Key::kRight]) {
    player.RotateRight(elapsed_time);
  }
}

void GameWindow::DrawMap(SDL::Renderer &renderer, const ColorMap &color_map) {
  for (int i = 0; i < Width(); ++i) {
    for (int j = 0; j < Height(); ++j) {
      renderer.SetColor(color_map[j * Width() + i]);
      renderer.DrawPixel(i, j);
    }
  }
  renderer.Present();
}

int GameWindow::Loop() {
  bool quit{};
  SDL::Event event;
  SDL::Renderer renderer(*this);
  auto tp1 = std::chrono::system_clock::now();
  auto tp2 = std::chrono::system_clock::now();
  while (!quit) {
    tp2 = std::chrono::system_clock::now();
    float elapsed_time = std::chrono::duration<float>(tp2 - tp1).count();
    tp1 = tp2;
    while (event.GetPoll()) {
      if (event.Type() == SDL::Event::kQuit) {
        quit = true;
      }
    }
    HandleInput(event, elapsed_time);
    auto view = RayCast(settings_.player, settings_.map, Width(), Height());
    DrawMap(renderer, view);
    SDL::Delay(20);
  }
  return EXIT_SUCCESS;
}

}  // namespace rcg