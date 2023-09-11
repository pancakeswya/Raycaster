#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include "raycast.h"
#include "adapt/SDL/event.h"
#include "adapt/SDL/render.h"
#include "adapt/SDL/window.h"
#include "util/settings.h"

namespace rcg {

class GameWindow final : public SDL::Window {
 public:
  GameWindow(std::string_view title, Settings settings, int width, int height)
      : SDL::Window(title, width, height), settings_(settings) {}
  int Loop();

 private:
  void DrawMap(SDL::Renderer& renderer, const ColorMap& color_map);
  void HandleInput(SDL::Event& event, float elapsed_time);
  Settings settings_;
};

}  // namespace rcg

#endif  // GAMEWINDOW_H_