#ifndef PLAYER_H_
#define PLAYER_H_

namespace rcg {

class Player {
 public:
  Player(float x, float y);

  [[nodiscard]] float X() const noexcept;
  [[nodiscard]] float Y() const noexcept;
  [[nodiscard]] float Pov() const noexcept;

  void SetX(float x) noexcept;
  void SetY(float y) noexcept;

  void MoveForward() noexcept;
  void MoveBackward() noexcept;
  void MoveLeft() noexcept;
  void MoveRight() noexcept;
  void RotateLeft() noexcept;
  void RotateRight() noexcept;
 private:
  float x_{},y_{};
  float pov_{};
};

inline Player::Player(float x, float y) : x_(x), y_(y) {}

inline float Player::X() const noexcept {
  return x_;
}

inline float Player::Y() const noexcept {
  return y_;
}

inline float Player::Pov() const noexcept {
  return pov_;
}

inline void Player::SetX(float x) noexcept {
  x_ = x;
}

inline void Player::SetY(float y) noexcept {
  y_ = y;
}

} // namespace rcg

#endif // PLAYER_H_