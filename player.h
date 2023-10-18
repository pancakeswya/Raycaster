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
  [[nodiscard]] float MoveSpeed() const noexcept;
  [[nodiscard]] float RotationSpeed() const noexcept;

  void SetX(float x) noexcept;
  void SetY(float y) noexcept;

  void SetMoveSpeed(float speed) noexcept;
  void SetRotationSpeed(float speed) noexcept;

  void MoveForwardX() noexcept;
  void MoveForwardY() noexcept;
  void MoveBackwardX() noexcept;
  void MoveBackwardY() noexcept;
  void MoveLeftX() noexcept;
  void MoveLeftY() noexcept;
  void MoveRightX() noexcept;
  void MoveRightY() noexcept;
  void RotateLeft() noexcept;
  void RotateRight() noexcept;
 private:
  float x_, y_;
  float dir_x_, dir_y_;
  float plane_x_, plane_y_;
  float rotation_speed_{}, movement_speed_{};
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

[[nodiscard]] inline float Player::MoveSpeed() const noexcept {
  return movement_speed_;
}

[[nodiscard]] inline float Player::RotationSpeed() const noexcept {
  return rotation_speed_;
}

inline void Player::SetX(float x) noexcept {
  x_ = x;
}

inline void Player::SetY(float y) noexcept {
  y_ = y;
}

} // namespace rcg

#endif // PLAYER_H_