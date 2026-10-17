#include "graphics.hpp"

void Rectangle::draw(setPixelFunc set_pixel) const {
    float c = cos(get_angle());
    float s = sin(get_angle());

    float half_x = _size.x * 0.5f;
    float half_y = _size.y * 0.5f;

    for (int y = -_size.y; y < _size.y; y++) {
        for (int x = -_size.x; x < _size.x; x++) {

            float source_x = c * x + s * y;
            float source_y = -s * x + c * y;

            if (source_x >= -half_x && source_x < half_x &&
                source_y >= -half_y && source_y < half_y) {

                set_pixel(x + get_center().x, y + get_center().y, get_color());
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