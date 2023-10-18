#include "player.h"
#include "config.h"

#include <cmath>

namespace rcg {

void Player::SetMoveSpeed(float speed) noexcept {
  movement_speed_ = speed;
}

void Player::SetRotationSpeed(float speed) noexcept {
  rotation_speed_ = speed;
}

void Player::MoveForwardX() noexcept {
  x_ += dir_x_ * movement_speed_;
}

void Player::MoveForwardY() noexcept {
  y_ += dir_y_ * movement_speed_;
}

void Player::MoveBackwardX() noexcept {
  x_ -= dir_x_ * movement_speed_;
}

void Player::MoveBackwardY() noexcept {
  y_ -= dir_y_ * movement_speed_;
}

void Player::MoveLeftX() noexcept {
  x_ -= plane_x_ * movement_speed_;
}

void Player::MoveLeftY() noexcept {
  y_ -= plane_y_ * movement_speed_;
}

void Player::MoveRightX() noexcept {
  x_ += plane_x_ * movement_speed_;
}

void Player::MoveRightY() noexcept {
  y_ += plane_y_ * movement_speed_;
}

void Player::RotateLeft() noexcept {
  float old_dir_x = dir_x_;
  dir_x_ = dir_x_ * std::cos(rotation_speed_) - dir_y_ * std::sin(rotation_speed_);
  dir_y_ = old_dir_x * std::sin(rotation_speed_) + dir_y_ * std::cos(rotation_speed_);
  float old_plane_x = plane_x_;
  plane_x_ = plane_x_ * std::cos(rotation_speed_) - plane_y_ * std::sin(rotation_speed_);
  plane_y_ = old_plane_x * std::sin(rotation_speed_) + plane_y_ * std::cos(rotation_speed_);
}

void Player::RotateRight() noexcept {
  float old_dir_x = dir_x_;
  dir_x_ = dir_x_ * std::cos(-rotation_speed_) - dir_y_ * std::sin(-rotation_speed_);
  dir_y_ = old_dir_x * std::sin(-rotation_speed_) + dir_y_ * std::cos(-rotation_speed_);
  float old_plane_x = plane_x_;
  plane_x_ = plane_x_ * std::cos(-rotation_speed_) - plane_y_ * std::sin(-rotation_speed_);
  plane_y_ = old_plane_x * std::sin(-rotation_speed_) + plane_y_ * std::cos(-rotation_speed_);
}


} // namespace rcg