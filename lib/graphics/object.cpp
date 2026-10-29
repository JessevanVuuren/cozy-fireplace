#include "graphics.hpp"

void Rectangle::draw(setPixelFunc set_pixel) const {
    float c = cos(get_angle());
    float s = sin(get_angle());

    Vec2 half = _size * 0.5f;
    Vec2 center = get_center();

    for (int y = -_size.y; y < _size.y; y++) {
        for (int x = -_size.x; x < _size.x; x++) {

            float source_x = c * x + s * y;
            float source_y = -s * x + c * y;

            if (source_x >= -half.x && source_x < half.x &&
                source_y >= -half.y && source_y < half.y) {

                set_pixel(x + center.x, y + center.y, get_color());
            }
        }
    }
}

void Circle::draw(setPixelFunc set_pixel) const {
    int r2 = _radius * _radius;

    int x_min = get_center().x - _radius;
    int x_max = get_center().x + _radius;
    int y_min = get_center().y - _radius;
    int y_max = get_center().y + _radius;

    for (int y = y_min; y <= y_max; ++y) {
        int dy = y - get_center().y;
        int dy2 = dy * dy;

        for (int x = x_min; x <= x_max; ++x) {
            int dx = x - get_center().x;
            int dx2 = dx * dx;

            if (dx2 + dy2 < r2) {
                set_pixel(x, y, get_color());
            }
        }
    }
}

Bitmap::Bitmap(Vec2 center, int width, int height) : Object(center, Color::Black), _width(width), _height(height) {
    _data = new Color[width * height];
}

void Bitmap::set_pixel(int x, int y, const Color &c) {
    if (_check_box(x, y)) {
        _data[y * _width + x] = c;
    }
    Serial.println("WARNING: x out of bounce");
}

const Color *Bitmap::get_pixel(int x, int y) {
    if (_check_box(x, y)) {
        return &_data[y * _width + x];
    }
    Serial.println("WARNING: y out of bounce");
    return nullptr;
}

void Bitmap::draw(setPixelFunc set_pixel) const {
    float c = cos(get_angle());
    float s = sin(get_angle());

    float half_x = _width * 0.5f;
    float half_y = _height * 0.5f;

    Vec2 center = get_center();

    for (int y = -_height; y < _height; y++) {
        for (int x = -_width; x < _width; x++) {

            float source_x = c * x + s * y;
            float source_y = -s * x + c * y;

            if (source_x >= -half_x && source_x < half_x &&
                source_y >= -half_y && source_y < half_y) {

                int offset_x = source_x + half_x;
                int offset_y = source_y + half_y;

                Color c = _data[offset_y * _width + offset_x];
                set_pixel(x + center.x, y + center.y, c);
            }
        }
    }
}
