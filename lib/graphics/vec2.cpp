#include "vec2.h"

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2() {
    x = 0.0f;
    y = 0.0f;
}

Vec2 Vec2::norm() const {
    float length = this->length();
    return *this / length;
}

Vec2 Vec2::min(const Vec2 &o) const {
    return Vec2(
        std::min(x, o.x),
        std::min(y, o.y));
}

Vec2 Vec2::max(const Vec2 &o) const {
    return Vec2(
        std::max(x, o.x),
        std::max(y, o.y));
}

float Vec2::cross(const Vec2 &o) const {
    return x * o.y - y * o.x;
}

float Vec2::length() const {
    return sqrtf(dot(*this));
}

float Vec2::length_sqr() const {
    return dot(*this);
}

float Vec2::dot(const Vec2 &o) const {
    return x * o.x + y * o.y;
}

float Vec2::distance(const Vec2 &o) const {
    return (*this - o).length();
}

float Vec2::distance_sqr(const Vec2 &o) const {
    return sqrtf((*this - o).length());
}

Vec2 Vec2::operator+(const Vec2 &o) const {
    return Vec2(x + o.x, y + o.y);
}

Vec2 Vec2::operator-(const Vec2 &o) const {
    return Vec2(x - o.x, y - o.y);
}

Vec2 Vec2::operator*(const Vec2 &o) const {
    return Vec2(x * o.x, y * o.y);
}

Vec2 Vec2::operator/(const Vec2 &o) const {
    return Vec2(x / o.x, y / o.y);
}

Vec2 Vec2::operator+(const float v) const {
    return Vec2(x + v, y + v);
}

Vec2 Vec2::operator-(const float v) const {
    return Vec2(x - v, y - v);
}

Vec2 Vec2::operator*(const float v) const {
    return Vec2(x * v, y * v);
}

Vec2 Vec2::operator/(const float v) const {
    return Vec2(x / v, y / v);
}

Vec2 &Vec2::operator+=(const Vec2 &c) {
    x += c.x;
    y += c.y;
    return *this;
}

Vec2 &Vec2::operator-=(const Vec2 &c) {
    x -= c.x;
    y -= c.y;
    return *this;
}

Vec2 &Vec2::operator*=(const Vec2 &c) {
    x *= c.x;
    y *= c.y;
    return *this;
}

Vec2 &Vec2::operator/=(const Vec2 &c) {
    x /= c.x;
    y /= c.y;
    return *this;
}

Vec2 &Vec2::operator+=(float v) {
    x += v;
    y += v;
    return *this;
}

Vec2 &Vec2::operator-=(float v) {
    x -= v;
    y -= v;
    return *this;
}

Vec2 &Vec2::operator*=(float v) {
    x *= v;
    y *= v;
    return *this;
}

Vec2 &Vec2::operator/=(float v) {
    x /= v;
    y /= v;
    return *this;
}