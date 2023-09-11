#ifndef MAP_H_
#define MAP_H_

#include "point.h"
#include <string>
#include <cmath>

namespace rcg {

enum Cell : char {
  kFloor = '.',
  kWall = '#'
};

class Map final {
public:
  Map(const char* map, int width, int height) 
    : width_(width), height_(height), map_(map) {}

  int Width() const noexcept { return width_; }

  int Height() const noexcept { return height_; }

  const char &operator[](int index) const noexcept { return map_[index]; }

  template<typename Tp>
  const char& At(Point<Tp> point) const noexcept {
    return map_[int(std::round(point.X())) * width_ + int(std::round(point.Y()))];
  }

private:
  int width_{}, height_{};
  std::string_view map_;
};

} // namespace rcg

#endif // MAP_H_