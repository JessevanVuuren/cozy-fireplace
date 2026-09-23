#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.hpp"
#include "object.hpp"
#include "vec2.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

using setPixelFunc = std::function<void(int, int, uint32_t)>;

class Graphics {
  public:
    Graphics(int width, int height, setPixelFunc set_pixel);

    int get_width() const { return _width; };
    int get_height() const { return _height; };

    void fill(Color color);
    void set_pixel(Vec2 pos, Color color);

    void circle(Vec2 pos, int radius, Color color);
    void square(Vec2 start, Vec2 end, Color color);

    void draw_objects();
    int add_object(Object &ob);

  private:
    setPixelFunc _set_pixel;

    std::vector<Object *> _objects;

    int _width;
    int _height;
    int _pixels;
};

#endif // GRAPHICS_H
