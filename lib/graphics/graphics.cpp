#include "graphics.h"

Graphics::Graphics(uint16_t width, uint16_t height) : _width(width), _height(height) {
    _pixels = width * height;
    _size = _pixels * 2;

    _buffer = new uint8_t[_size]();
}

Graphics::~Graphics() {
    delete[] _buffer;
}

uint8_t *Graphics::get_buffer() const {
    return _buffer;
}

uint16_t Graphics::buffer_size() const {
    return _size;
}

uint8_t Graphics::get_width() const {
    return _width;
}

uint8_t Graphics::get_height() const {
    return _height;
}

void Graphics::fill(uint16_t color) {
    for (uint16_t i = 0; i < _size; i += 2) {
        _buffer[i] = color >> 8;
        _buffer[i + 1] = color & 0xFF;
    }
}

void Graphics::set_pixel(uint8_t x, uint8_t y, uint16_t color) {
    uint16_t index = y * (_height) + x;

    _buffer[index * 2] = color >> 8;
    _buffer[index * 2 + 1] = color & 0xFF;
}

void Graphics::circle(uint8_t pos_x, uint8_t pos_y, uint16_t radius, uint16_t color) {
    for (uint16_t y = 0; y < _height; y++) {
        for (uint16_t x = 0; x < _width; x++) {
            uint16_t distance = std::sqrt(std::pow(pos_x - x, 2) + std::pow(pos_y - y, 2));

            if (distance < radius) {
                set_pixel(x, y, color);
            }
        }
    }
}