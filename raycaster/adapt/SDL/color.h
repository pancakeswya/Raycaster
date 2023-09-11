#ifndef COLOR_H_
#define COLOR_H_

namespace SDL {

struct Color final {
  int r, g, b, a;
};

namespace {

constexpr Color Black{0, 0, 0, 255}, White{255, 255, 255, 255},
    LightGray{195, 195, 195, 255}, DarkGray{127, 127, 127, 255},
    Pink{255, 174, 201, 255}, Red{255, 0, 0, 255}, Maroon{136, 0, 21, 255},
    Magenta{255, 0, 255, 255}, Brown{185, 122, 87, 255},
    Yellow{255, 255, 0, 255}, LightGreen{128, 255, 128, 255},
    Green{0, 255, 0, 255}, DarkGreen{0, 128, 0, 255},
    LightBlue{128, 128, 255, 255}, Blue{0, 0, 255, 255},
    DarkBlue{0, 0, 128, 255}, Purple{163, 73, 164, 255};

}  // namespace

}  // namespace SDL

#endif  // COLOR_H_