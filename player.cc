#include "player.h"
#include "config.h"

#include <cmath>

namespace rcg {

void Player::MoveForwardX(float step) noexcept {
  x_ += dir_x_ * step;
}

void Player::MoveForwardY(float step) noexcept {
  y_ += dir_y_ * step;
}

void Player::MoveBackwardX(float step) noexcept {
  x_ -= dir_x_ * step;
}

void Player::MoveBackwardY(float step) noexcept {
  y_ -= dir_y_ * step;
}

void Player::MoveLeftX(float step) noexcept {
  x_ -= plane_x_ * step;
}

void Player::MoveLeftY(float step) noexcept {
  y_ -= plane_y_ * step;
}

void Player::MoveRightX(float step) noexcept {
  x_ += plane_x_ * step;
}

void Player::MoveRightY(float step) noexcept {
  y_ += plane_y_ * step;
}

void Player::RotateX(float degree) noexcept {

  float old_dir_x = dir_x_;
  dir_x_ = dir_x_ * std::cos(degree) - dir_y_ * std::sin(degree);
  dir_y_ = old_dir_x * std::sin(degree) + dir_y_ * std::cos(degree);
  float old_plane_x = plane_x_;
  plane_x_ = plane_x_ * std::cos(degree) - plane_y_ * std::sin(degree);
  plane_y_ = old_plane_x * std::sin(degree) + plane_y_ * std::cos(degree);
}

} // namespace rcg