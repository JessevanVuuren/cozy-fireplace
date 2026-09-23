#ifndef OBJECT_H
#define OBJECT_H

#include "color.hpp"
#include "graphics.hpp"
#include "vec2.hpp"

#include <functional>

using setPixelFunc = std::function<void(int, int, uint32_t)>;

enum class ObjectType {
    Rectangle,
};

class Object {
  public:
    Object(Vec2 pos, Vec2 size, Color color) : _center(pos), _size(size), _color(color) {};

    Vec2 get_pos() const { return _center; }
    Vec2 get_size() const { return _size; }
    Color get_color() const { return _color; }

    void set_id(int id) { _id = id; }
    void set_angle(float angle) { _angle = angle; }
    void set_color(const Color &color) { _color = color; }

    void draw_object(setPixelFunc set_pixel) const;

  private:
    void draw_rectangle(setPixelFunc set_pixel) const;

    int _id;

    Vec2 _center;
    Vec2 _size;

    float _angle;

    Color _color;
    ObjectType _type;
};

#endif // OBJECT_H
