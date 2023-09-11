#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL2/SDL.h>

#include <cassert>
#include <cstdlib>
#include <string>

namespace SDL {

class Renderer;

class Window {
  friend class Renderer;

 public:
  Window(std::string_view title, int width, int height) : width_(width), height_(height) {
    SDL_Init(SDL_INIT_VIDEO);
    window_ = SDL_CreateWindow(title.data(), 0, 0, width, height, 0);
    assert(window_);
  }

  ~Window() {
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  int Width() const noexcept {
    return width_;
  }

  int Height() const noexcept {
    return height_;
  }

 private:
  SDL_Window *window_{};
  int width_{}, height_{};
};

}  // namespace SDL

#endif  // WINDOW_H_