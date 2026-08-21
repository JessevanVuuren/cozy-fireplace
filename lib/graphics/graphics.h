#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <cstdint>
#include <cmath>

class Graphics {
  public:
    Graphics(uint16_t width, uint16_t height);
    ~Graphics();

    uint8_t *get_buffer() const;
    uint16_t buffer_size() const;

    uint8_t get_width() const;
    uint8_t get_height() const;

    void fill(uint16_t color);

    void set_pixel(uint8_t x, uint8_t y, uint16_t color);
    void circle(uint8_t x, uint8_t y, uint16_t radius, uint16_t color);

  private:
    uint16_t _size;
    uint16_t _pixels;
    uint8_t *_buffer;

    uint16_t _width;
    uint16_t _height;
};

#endif // GRAPHICS_H
