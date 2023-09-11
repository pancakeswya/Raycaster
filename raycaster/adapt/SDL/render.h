#ifndef RENDER_H_
#define RENDER_H_

#include <SDL2/SDL_render.h>

#include <cassert>

#include "color.h"
#include "window.h"

namespace SDL {

class Renderer final {
 public:
  Renderer() = default;
  Renderer(Window &win) {
    renderer_ = SDL_CreateRenderer(
        win.window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    assert(renderer_);
    SDL_SetRenderDrawColor(renderer_, SDL::White.r, SDL::White.g, SDL::White.b, SDL::White.a);
    SDL_RenderClear(renderer_);
  }

  void Clear() {
    SDL_RenderClear(renderer_);
  }

  void SetColor(const Color &color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
  }

  void DrawRect(int x, int y, int w, int h) {
    SDL_Rect rect_to_draw{x, y, w, h};
    SDL_RenderFillRect(renderer_, &rect_to_draw);
    SDL_RenderDrawRect(renderer_, &rect_to_draw);
  }

  void DrawPixel(int x, int y) {
    SDL_RenderDrawPoint(renderer_, x, y);
  }

  void Present() { SDL_RenderPresent(renderer_); }

  ~Renderer() { SDL_DestroyRenderer(renderer_); }

 private:
  SDL_Renderer *renderer_{};
};

}  // namespace SDL

#endif  // RENDER_H_