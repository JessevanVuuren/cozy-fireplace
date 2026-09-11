#include "SSD1351.h"
#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include <cstdint>

SSD1351::SSD1351(int cs, int dc, int rst, unsigned long hz, ColorLevel color_bit)
    : _cs(cs), _dc(dc), _rst(rst), _hz(hz), _color_bit(color_bit) {}

SSD1351::~SSD1351() {
    if (_owns_buffer) {
        delete[] _buffer;
    }
}

void SSD1351::start() {
    _start();

    create_buffer();
    _owns_buffer = true;
}

void SSD1351::start(uint8_t *buffer, int buffer_size) {
    _start();

    _buffer = buffer;
    _owns_buffer = false;
    _buffer_size = buffer_size;
}

void SSD1351::_start() {
    reset();

    init_gpio_pins();
    init_SPI();
    init_oled_regi();

    display_on();
}

void SSD1351::create_buffer() {
    if (_buffer != nullptr) {
        return;
    }

    _owns_buffer = true;

    _buffer_size = _get_buffer_size();
    _buffer = new uint8_t[_buffer_size]();
}

int SSD1351::_get_buffer_size() {
    switch (_color_bit) {
    case 0:
        return SSD1351_PIXELS * 2;
    case 1:
        return SSD1351_PIXELS * 2;
    case 2:
        return SSD1351_PIXELS * 3;
    }
}

void SSD1351::init_gpio_pins() {
    pinMode(_rst, OUTPUT);
    pinMode(_cs, OUTPUT);
    pinMode(_dc, OUTPUT);
}

void SSD1351::reset() {
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(100);
    digitalWrite(_rst, HIGH);
    delay(100);
}

void SSD1351::clear_buffer() {
    for (int i = 0; i < _buffer_size; i++) {
        _buffer[i] = 0x00;
    }
}

void SSD1351::fill_buffer(uint32_t color) {
    for (int y = 0; y < SSD1351_HEIGHT; y++) {
        for (int x = 0; x < SSD1351_WIDTH; x++) {
            set_pixel(x, y, color);
        }
    }
}

void SSD1351::render() {
    digitalWrite(_cs, LOW);
    digitalWrite(_dc, LOW);

    SPI.transfer(0x15);
    SPI.transfer(0);
    SPI.transfer(127);

    SPI.transfer(0x75);
    SPI.transfer(0);
    SPI.transfer(127);

    SPI.transfer(0x5C);

    digitalWrite(_dc, HIGH);

    SPI.transfer(_buffer, _buffer_size);

    digitalWrite(_cs, HIGH);
}

void SSD1351::set_pixel(int x, int y, uint32_t color) {
    int index = (127-y) * SSD1351_HEIGHT + (127-x);

    switch (_color_bit) {
    case 0: {
        int offset = index * 2;

        int r = ((color >> 5) & 0x07) * 31 / 7;
        int g = ((color >> 2) & 0x07) * 63 / 7;
        int b = ((color >> 0) & 0x03) * 31 / 3;

        uint16_t bit_color = (r << 11) | (g << 5) | b;

        _buffer[offset + 0] = bit_color >> 8;
        _buffer[offset + 1] = bit_color & 0xFF;
    } break;
    case 1: {
        int offset = index * 2;

        _buffer[offset + 0] = color >> 8;
        _buffer[offset + 1] = color & 0xFF;
    } break;
    case 2: {
        int offset = index * 3;

        _buffer[offset + 0] = (color >> 12) & 0x3F;
        _buffer[offset + 1] = (color >> 6) & 0x3F;
        _buffer[offset + 2] = color & 0x3F;
    } break;
    }
}

uint32_t SSD1351::get_pixel(int x, int y) const {
    int index = y * SSD1351_HEIGHT + x;
    int offset = index * 2;

    int color_1 = _buffer[offset + 0];
    int color_2 = _buffer[offset + 1];

    return (color_1 << 8) | color_2;
}

void SSD1351::write_regi(int reg) {
    digitalWrite(_dc, LOW);
    digitalWrite(_cs, LOW);
    SPI.transfer(reg);
    digitalWrite(_cs, HIGH);
}

void SSD1351::write_data(int data) {
    digitalWrite(_dc, HIGH);
    digitalWrite(_cs, LOW);
    SPI.transfer(data);
    digitalWrite(_cs, HIGH);
}

void SSD1351::init_SPI() {
    SPI.begin();
    SPI.beginTransaction(SPISettings(_hz, SPI_MSBFIRST, SPI_MODE3));
}

void SSD1351::display_off() { write_regi(0xAE); }

void SSD1351::display_on() { write_regi(0xAF); }

void SSD1351::init_oled_regi() {
    write_regi(SSD1351_COMMAND_LOCK);
    write_data(0x12);
    write_regi(SSD1351_COMMAND_LOCK);
    write_data(0xB1);

    write_regi(SSD1351_DISPLAY_OFF);
    write_regi(SSD1351_DISPLAY_ALL_OFF);

    write_regi(SSD1351_SET_COLUMN_ADDRESS);
    write_data(0x00);
    write_data(0x7f);
    write_regi(SSD1351_SET_ROW_ADDRESS);
    write_data(0x00);
    write_data(0x7f);

    write_regi(SSD1351_CLOCK_DIV);
    write_data(0xF1);

    write_regi(SSD1351_MUX_RATIO);
    write_data(0x7F);

    write_regi(SSD1351_SET_REMAP_COLOR);
    switch (_color_bit) {
    case 0: {
        write_data(0x74);
    } break;
    case 1: {
        write_data(0x74);
    } break;
    case 2: {
        write_data(0xB4);
    } break;
    }

    write_regi(SSD1351_START_LINE);
    write_data(0x00);

    write_regi(SSD1351_DISPLAY_OFFSET);
    write_data(0x00);

    write_regi(SSD1351_FUNCTION_SELECT);
    write_regi(0x01);

    write_regi(SSD1351_SET_VSL);
    write_data(0xA0);
    write_data(0xB5);
    write_data(0x55);

    write_regi(SSD1351_CONTRAST_ABC);
    write_data(0xC8);
    write_data(0x80);
    write_data(0xC0);

    write_regi(SSD1351_CONTRAST_MASTER);
    write_data(0x0F);

    write_regi(SSD1351_PRE_CHARGE);
    write_data(0x32);

    write_regi(SSD1351_DISPLAY_ENHANCE);
    write_data(0xA4);
    write_data(0x00);
    write_data(0x00);

    write_regi(SSD1351_PRE_CHARGE_LEVEL);
    write_data(0x17);

    write_regi(SSD1351_PRE_CHARGE_2);
    write_data(0x01);

    write_regi(SSD1351_VCOMH);
    write_data(0x05);

    write_regi(SSD1351_NORMAL_DISPLAY);
}
