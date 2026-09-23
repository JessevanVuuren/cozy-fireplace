#include "graphics.hpp"

void Object::draw_object(setPixelFunc set_pixel) const {
    switch (_type) {
    case ObjectType::Rectangle:
        draw_rectangle(set_pixel);
        break;
    }
}

void Object::draw_rectangle(setPixelFunc set_pixel) const {
    float c = cos(_angle);
    float s = sin(_angle);

    float half_x = _size.x * 0.5f;
    float half_y = _size.y * 0.5f;

    for (int y = -_size.y; y < _size.y; y++) {
        for (int x = -_size.x; x < _size.x; x++) {

            float source_x = c * x + s * y;
            float source_y = -s * x + c * y;

            if (source_x >= -half_x && source_x < half_x &&
                source_y >= -half_y && source_y < half_y) {

                set_pixel(x + _center.x, y + _center.y, _color.to_16bit());
            }
        }
    }
}