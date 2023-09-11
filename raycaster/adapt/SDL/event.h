#ifndef EVENT_H_
#define EVENT_H_

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_timer.h>

namespace SDL {

namespace Key {
constexpr int kNum = SDL_NUM_SCANCODES;
constexpr int kUp = SDL_SCANCODE_UP;
constexpr int kDown = SDL_SCANCODE_DOWN;
constexpr int kLeft = SDL_SCANCODE_LEFT;
constexpr int kRight = SDL_SCANCODE_RIGHT;
}  // namespace Key

class Event final {
 public:
  static constexpr unsigned int kQuit = SDL_QUIT;
  static constexpr unsigned int kKeyDown = SDL_KEYDOWN;
  static constexpr unsigned int kKeyUp = SDL_KEYUP;

  int GetPoll() noexcept {
    return SDL_PollEvent(&event_);
  }

  const unsigned char* GetKeyboardState() noexcept {
    return SDL_GetKeyboardState(nullptr);
  }

  unsigned int KeyRepeated() noexcept { return event_.key.repeat; }

  int Key() noexcept { return event_.key.keysym.scancode; }

  unsigned int Type() noexcept { return event_.type; }

  void Wait() noexcept { SDL_WaitEvent(&event_); }

 private:
  SDL_Event event_;
};

inline void Delay(unsigned int time) {
  SDL_Delay(time);
}

}  // namespace SDL

#endif  // EVENT_H_