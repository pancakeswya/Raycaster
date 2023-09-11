#ifndef POINT_H_
#define POINT_H_

namespace rcg {

template<typename Tp>
class Point {
public:
  Point() = default;
  explicit Point(Tp x, Tp y) : x_(x), y_(y) {}

  Tp X() const noexcept { return x_; }

  Tp Y() const noexcept { return y_; }

  void SetX(Tp value) noexcept { x_ = value; }

  void SetY(Tp value) noexcept { y_ = value; }

protected:
  Tp x_{}, y_{};
};

} // namespace rcg

#endif // POINT_H_