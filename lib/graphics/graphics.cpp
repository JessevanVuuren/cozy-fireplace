#include "graphics.hpp"

Graphics::Graphics(int width, int height, setPixelFunc set_pixel) : _width(width), _height(height), _set_pixel(set_pixel) {
    _pixels = width * height;
}

void Graphics::fill(Color color) {
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
            set_pixel(x, y, color);
        }
    }
}

void Graphics::set_pixel(Vec2 pos, Color color) {
    set_pixel(pos.x, pos.y, color);
}

void Graphics::set_pixel(int x, int y, Color color) {
    _set_pixel(x, y, color);
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

void Graphics::square(Vec2 start, Vec2 stop, Color color) {
    Vec2 min = start.min(stop);
    Vec2 max = start.max(stop);

    Vec2 size = max - min;

    for (int y = 0; y < size.y; y++) {
        for (int x = 0; x < size.x; x++) {
            set_pixel(Vec2(x, y) + start, color);
        }
    }
}

void Graphics::draw_objects() {
    for (const Object *ob : _objects) {
        ob->draw(_set_pixel);
    }
}

void Graphics::add_object(Object &ob) {
    _objects.push_back(&ob);
}

void Graphics::set_text(Vec2 start, Color color, String text) {
    for (size_t i = 0; i < text.length(); i++) {
        int index = char_font_index(text[i]) * FONT_HEIGHT;

        int offset_x = start.x + FONT_WIDTH * i;
        int offset_y = start.y;

        for (size_t y = 0; y < FONT_HEIGHT; y++) {
            uint8_t letter_row = font[index + y];

            for (size_t x = 0; x < FONT_WIDTH; x++) {
                int pixel = FONT_WIDTH - x - 1;

                if (letter_row & 1 << (pixel)) {
                    set_pixel(offset_x + x, offset_y + y, color);
                }
            }
        }
    }
}