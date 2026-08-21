#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <algorithm>
#include <math.h>

class Vec2 {
  public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);

    static Vec2 one() { return Vec2(1.0f, 1.0f); };
    static Vec2 zero() { return Vec2(0.0f, 0.0f); };
    static Vec2 splat(float v) { return Vec2(v, v); };

    static Vec2 up() { return Vec2(1.0f, 0.0f); };
    static Vec2 down() { return Vec2(-1.0f, 0.0f); };
    static Vec2 left() { return Vec2(0.0f, -1.0f); };
    static Vec2 right() { return Vec2(0.0f, 1.0f); };

    Vec2 norm() const;
    Vec2 min(const Vec2 &o) const;
    Vec2 max(const Vec2 &o) const;
    
    float length() const;
    float length_sqr() const;
    float cross(const Vec2 &o) const;

    float dot(const Vec2 &o) const;
    float distance(const Vec2 &o) const;
    float distance_sqr(const Vec2 &o) const;

    Vec2 operator+(const Vec2 &o) const;
    Vec2 operator-(const Vec2 &o) const;
    Vec2 operator*(const Vec2 &o) const;
    Vec2 operator/(const Vec2 &o) const;

    Vec2 operator+(const float v) const;
    Vec2 operator-(const float v) const;
    Vec2 operator*(const float v) const;
    Vec2 operator/(const float v) const;

    Vec2 &operator+=(const Vec2 &o);
    Vec2 &operator-=(const Vec2 &o);
    Vec2 &operator*=(const Vec2 &o);
    Vec2 &operator/=(const Vec2 &o);

    Vec2 &operator+=(float v);
    Vec2 &operator-=(float v);
    Vec2 &operator*=(float v);
    Vec2 &operator/=(float v);

  private:
};

#endif // VECTOR_2D_H
