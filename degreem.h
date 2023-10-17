#ifndef DEGREEM_H_
#define DEGREEM_H_

#include <cmath>

namespace rcg::dgm {

constexpr float kPi = 3.141592653589793116f;

inline float DegreeToRadians(float degrees) {
  return kPi * degrees / 180.0f;
}

inline float ToValidDegrees(float degrees) {
  return std::fmod(360.0f + std::fmod(degrees, 360.0f), 360.0f);
}


} //

#endif // DEGREEM_H_