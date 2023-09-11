#ifndef PLAYER_H_
#define PLAYER_H_

#include "point.h"
#include <cmath>

namespace rcg {

class Player final : public Point<float> {
  using Coords = Point<float>;
public:
  explicit Player(float x, float y, float fov, float speed) 
    : Point<float>(x,y), fov_(fov), speed_(speed) {}

  float A() const noexcept {
    return a_;
  }

  float Fov() const noexcept {
    return fov_;
  }

  float Speed() const noexcept {
    return speed_;
  }

  void MoveUp(float e_time) noexcept;
  void MoveDown(float e_time) noexcept;
  void RotateRight(float e_time) noexcept;
  void RotateLeft(float e_time) noexcept;

private:
  Coords GetMoveStep(float e_time) noexcept;
  float a_{};
  float fov_;
  float speed_;
};

inline Player::Coords Player::GetMoveStep(float e_time) noexcept {
  return Coords(std::sin(a_) * speed_ * e_time,
                std::cos(a_) * speed_ * e_time);
}


inline void Player::MoveUp(float e_time) noexcept {
  Coords step = GetMoveStep(e_time);
  x_ += step.X();
  y_ += step.Y();
}

inline void Player::MoveDown(float e_time) noexcept {
  Coords step = GetMoveStep(e_time);
  x_ -= step.X();
  y_ -= step.Y();
}

inline void Player::RotateLeft(float e_time) noexcept {
  a_ -= (speed_ * 0.25f) * e_time;
}

inline void Player::RotateRight(float e_time) noexcept {
  a_ += (speed_ * 0.25f) * e_time;
}

} // namespace rcg

#endif // PLAYER_H_