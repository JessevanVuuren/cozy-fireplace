#include "graphics.h"

Graphics::Graphics(int width, int height, setPixelFunc set_pixel) : _width(width), _height(height), _set_pixel(set_pixel) {
    _pixels = width * height;
}

void Graphics::fill(Color color) {
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            _set_pixel(x, y, color.to_16bit());
        }
    }
}

void Graphics::set_pixel(Vec2 pos, Color color) {
    _set_pixel(pos.x, pos.y, color.to_16bit());
}

void Graphics::circle(Vec2 pos, int radius, Color color) {
    int r2 = radius * radius;

    int x_min = fmaxf(0, pos.x - radius);
    int x_max = fminf(_width - 1, pos.x + radius);
    int y_min = fmaxf(0, pos.y - radius);
    int y_max = fminf(_height - 1, pos.y + radius);

    for (int y = y_min; y <= y_max; ++y) {
        int dy = y - pos.y;
        int dy2 = dy * dy;

        for (int x = x_min; x <= x_max; ++x) {
            int dx = x - pos.x;
            int dx2 = dx * dx;

            if (dx2 + dy2 < r2) {
                set_pixel(Vec2(x, y), color);
            }
        }
    }
}