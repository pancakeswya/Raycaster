#ifndef CONFIG_H_
#define CONFIG_H_

#include <chrono>
#include <string>

namespace rcg::config {

constexpr int kCellSize = 64;
constexpr int kMapCellSize = 8;
constexpr int kMapGridCellSize = 16;
constexpr int kWindowWidth = 1280;
constexpr int kWindowHeight = 720;
constexpr int kMapWidth = 40;
constexpr int kMapHeight = 24;
constexpr int kFov = 60;

constexpr int kPlayerStartX = 65;
constexpr int kPlayerStartY = 65;

constexpr float kMoveSpeed = 2.0f;
constexpr float kRenderDistance = 1024.0f;
constexpr float kRotationSpeed = 2.0f;

constexpr std::chrono::microseconds kFrameDuration(16667);

constexpr std::string_view kMapPath = "Resources/Images/MapSketch.png";
constexpr std::string_view kMapGridCellPath = "Resources/Images/MapGridCell.png";
inline std::string kMapWallPath = "Resources/Images/MapWall" + std::to_string(config::kMapCellSize) + ".png";
inline std::string kMapPlayerPath = "Resources/Images/MapPlayer" + std::to_string(config::kMapCellSize) + ".png";
inline std::string kIgWallPath = "Resources/Images/Wall" + std::to_string(config::kCellSize) + ".png";

} // namespace rcg::config


#endif // CONFIG_H_