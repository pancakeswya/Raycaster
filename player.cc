#include "player.h"
#include "config.h"
#include "degreem.h"

namespace rcg {

void Player::MoveForward() noexcept {
  pov_ = dgm::ToValidDegrees(pov_);
  x_ += config::kMoveSpeed  * std::cos(dgm::DegreeToRadians(pov_));
  y_ -= config::kMoveSpeed * std::sin(dgm::DegreeToRadians(pov_));
}

void Player::MoveBackward() noexcept {
  pov_ = dgm::ToValidDegrees(pov_);
  x_ -= config::kMoveSpeed * std::cos(dgm::DegreeToRadians(pov_));
  y_ += config::kMoveSpeed * std::sin(dgm::DegreeToRadians(pov_));
}

void Player::MoveLeft() noexcept {
  pov_ = dgm::ToValidDegrees(pov_);
  x_ += config::kMoveSpeed * std::cos(dgm::DegreeToRadians(dgm::ToValidDegrees(90 + pov_)));
  y_ -= config::kMoveSpeed * std::sin(dgm::DegreeToRadians(dgm::ToValidDegrees(90 + pov_)));
}

void Player::MoveRight() noexcept {
  pov_ = dgm::ToValidDegrees(pov_);
  x_ += config::kMoveSpeed * std::cos(dgm::DegreeToRadians(dgm::ToValidDegrees(pov_ - 90)));
  y_ -= config::kMoveSpeed * std::sin(dgm::DegreeToRadians(dgm::ToValidDegrees(pov_ - 90)));
}

void Player::RotateLeft() noexcept {
  pov_ += config::kRotationSpeed;
}

void Player::RotateRight() noexcept {
  pov_ -= config::kRotationSpeed;
}

} // namespace rcg