#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "color.h"
#include "vec2.h"

#include <functional>
#include <algorithm>
#include <cstdint>
#include <cmath>

using setPixelFunc = std::function<void(int, int, uint32_t)>;

class Graphics {
  public:
    Graphics(int width, int height, setPixelFunc set_pixel);

    int get_width() const { return _width; };
    int get_height() const { return _height; };

    void fill(Color color);
    void set_pixel(Vec2 pos, Color color);
    void circle(Vec2 pos, int radius, Color color);

  private:
    setPixelFunc _set_pixel;

    int _width;
    int _height;
    int _pixels;
};

#endif // GRAPHICS_H
