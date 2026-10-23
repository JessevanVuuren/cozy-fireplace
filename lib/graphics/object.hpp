#ifndef OBJECT_H
#define OBJECT_H

#include "color.hpp"
#include "graphics.hpp"
#include "vec2.hpp"

#include <functional>

using setPixelFunc = std::function<void(int, int, Color)>;

class Object {
  public:
    virtual ~Object() = default;

    virtual void draw(setPixelFunc set_pixel) const = 0;

    const float get_angle() const { return _angle; }
    const Color &get_color() const { return _color; }
    const Vec2 &get_center() const { return _center; }

    void set_angle(float angle) { _angle = angle; }
    void set_color(const Color &color) { _color = color; }
    void set_center(const Vec2 &center) { _center = center; }

  protected:
    Object(Vec2 center, Color color, float angle = 0.0f) : _center(center), _color(color), _angle(angle) {};

  private:
    Vec2 _center;
    Color _color;
    float _angle;
};

class Bitmap : public Object {
  public:
    Bitmap(Vec2 center, int width, int height);
    ~Bitmap() { delete[] _data; };

    void draw(setPixelFunc set_pixel) const override;

    int get_height() const { return _height; };
    int get_width() const { return _width; };
    Vec2 get_size() const { return Vec2(_width, _height); };

    void set_height(int height) { _height = height; };
    void set_width(int width) { _width = width; };

    Color *get_data() const { return _data; };

    void set_pixel(int x, int y, const Color &color);
    const Color &get_pixel(int x, int y);

  private:
    int _width;
    int _height;
    Color *_data;

    bool _check_box(int x, int y) { return _check_box_x(x) && _check_box_y(y); };
    bool _check_box_x(int x) { return x >= 0 && x < _width; };
    bool _check_box_y(int y) { return y >= 0 && y < _height; };
};

class Rectangle : public Object {
  public:
    Rectangle(Vec2 center, Vec2 size, Color color) : Object(center, color), _size(size) {};

    void draw(setPixelFunc set_pixel) const override;

    const Vec2 &get_size() const { return _size; };
    void set_size(const Vec2 &size) { _size = size; };

  private:
    Vec2 _size;
};

class Circle : public Object {
  public:
    Circle(Vec2 center, float radius, Color color) : Object(center, color), _radius(radius) {};

    void draw(setPixelFunc set_pixel) const override;

    const float &get_radius() const { return _radius; };
    void set_radius(const float &radius) { _radius = radius; };

  private:
    float _radius;
};

#endif // OBJECT_H
