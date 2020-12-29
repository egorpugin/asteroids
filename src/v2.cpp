#include "asteroids/v2.h"
#include <math.h>

void v2f::rotate(float angle) {
  auto x1 = x * cos(angle) - y * sin(angle);
  auto y1 = x * sin(angle) + y * cos(angle);
  x = x1;
  y = y1;
}

void v2f::scale(float scalar) {
  x *= scalar;
  y *= scalar;
}

float v2f::dist_squared(const v2f &v2) const {
  auto x1 = v2.x - x;
  auto y1 = v2.y - y;
  return x1*x1 + y1*y1;
}
