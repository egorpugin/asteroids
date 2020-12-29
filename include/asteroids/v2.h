/**
  Module that defines 2D vector type and operations.
*/
#ifndef ASTEROIDS_V2_H
#define ASTEROIDS_V2_H

/// 2D vector of floats
struct v2f {
  float x;
  float y;

  void rotate(float angle);
  void scale(float scalar);
  float dist_squared(const v2f &) const;

  v2f &operator+=(const v2f &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }
};

#endif /* ASTEROIDS_V2_H */
