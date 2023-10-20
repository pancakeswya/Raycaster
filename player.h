#ifndef PLAYER_H_
#define PLAYER_H_

namespace rcg {

class Player {
 public:
  Player(float x, float y,
         float dir_x, float dir_y,
         float plane_x, float plane_y);
  [[nodiscard]] float X() const noexcept;
  [[nodiscard]] float Y() const noexcept;
  [[nodiscard]] float DirX() const noexcept;
  [[nodiscard]] float DirY() const noexcept;
  [[nodiscard]] float PlaneX() const noexcept;
  [[nodiscard]] float PlaneY() const noexcept;

  void SetX(float x) noexcept;
  void SetY(float y) noexcept;

  void MoveForwardX(float step) noexcept;
  void MoveForwardY(float step) noexcept;
  void MoveBackwardX(float step) noexcept;
  void MoveBackwardY(float step) noexcept;
  void MoveLeftX(float step) noexcept;
  void MoveLeftY(float step) noexcept;
  void MoveRightX(float step) noexcept;
  void MoveRightY(float step) noexcept;
  void RotateX(float degree) noexcept;
 private:
  float x_, y_;
  float dir_x_, dir_y_;
  float plane_x_, plane_y_;
};

inline Player::Player(float x, float y,
                      float dir_x, float dir_y,
                      float plane_x, float plane_y)
    : x_(x), y_(y),
      dir_x_(dir_x), dir_y_(dir_y),
      plane_x_(plane_x), plane_y_(plane_y) {}

inline float Player::X() const noexcept {
  return x_;
}

inline float Player::Y() const noexcept {
  return y_;
}

[[nodiscard]] inline float Player::DirX() const noexcept {
  return dir_x_;
}

[[nodiscard]] inline float Player::DirY() const noexcept {
  return dir_y_;
}

[[nodiscard]] inline float Player::PlaneX() const noexcept {
  return plane_x_;
}

[[nodiscard]] inline float Player::PlaneY() const noexcept {
  return plane_y_;
}

inline void Player::SetX(float x) noexcept {
  x_ = x;
}

inline void Player::SetY(float y) noexcept {
  y_ = y;
}

} // namespace rcg

#endif // PLAYER_H_