#include "gamewindow.h"

constexpr int kWindowWidth = 1320, kWindowHeight = 660;
constexpr int kMapWidth = 16, kMapHeight = 16;
constexpr float kPlayerX = 14.7f, kPlayerY = 5.09f;
constexpr float kSpeed = 6.0f;
constexpr float kPov = 0.785398f;
constexpr char kMap[] = {
    "#########......."
    "#..............."
    "#.......########"
    "#..............#"
    "#......##......#"
    "#......##......#"
    "#..............#"
    "###............#"
    "##.............#"
    "#......####..###"
    "#......#.......#"
    "#......#.......#"
    "#..............#"
    "#......#########"
    "#..............#"
    "################"};


int main() {
  rcg::Settings settings{rcg::Player(kPlayerX, kPlayerY, kPov, kSpeed),
                         rcg::Map(kMap, kMapWidth, kMapHeight)};
  rcg::GameWindow win("Raycast", settings, kWindowWidth, kWindowHeight);
  return win.Loop();
}
