#include "color.hpp"

Color::Color() : red(0.0f), green(0.0f), blue(0.0f) {}

Color::Color(float value) : red(value), green(value), blue(value) {}

Color::Color(float red, float green, float blue) : red(red), green(green), blue(blue) {}

const Color Color::Red = Color(1.0f, 0.0f, 0.0f);
const Color Color::Green = Color(0.0f, 1.0f, 0.0f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f);
const Color Color::White = Color(1.0f, 1.0f, 1.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f);

uint8_t Color::to_8bit() const {
    uint8_t r = red * 7;
    uint8_t g = green * 7;
    uint8_t b = blue * 3;

    return (r << 5) | (g << 2) | b;
}

uint16_t Color::to_16bit() const {
    uint8_t r = red * 31;
    uint8_t g = green * 63;
    uint8_t b = blue * 31;

    return (r << 11) | (g << 5) | b;
}

uint32_t Color::to_18bit() const {
    uint32_t r = red * 63;
    uint32_t g = green * 63;
    uint32_t b = blue * 63;

    return (r << 12) | (g << 6) | b;
}

const char *Color::to_string() const {
    std::string str = "RGB: " +
                      std::to_string(red) + ", " +
                      std::to_string(green) + ", " +
                      std::to_string(blue);

    return str.c_str();
}

Color Color::to_gamma() const {
    return Color(*this).pow(1.0f / 2.2f);
}

float Color::_convert_sRGB(float value) const {
    if (value <= 0.0031308f) {
        return value * 12.92f;
    }
    return 1.055f * std::pow(value, 1.0f / 2.4f) - 0.055f;
}

Color Color::to_sRGB() const {
    Color c = *this;

    c.red = _convert_sRGB(red);
    c.green = _convert_sRGB(green);
    c.blue = _convert_sRGB(blue);

    return c;
}

Color Color::to_RGB(ColorHSV hsv) {
    float c = hsv.value * hsv.saturation;
    float h = hsv.hue / 60.0f;
    float x = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
    float m = hsv.value - c;

    Color color = _hsl_hsv_to_rgb(h, c, x);

    return color + m;
}

Color Color::to_RGB(ColorHSL hsl) {
    
    float c = (1.0f - fabsf(2.0f * hsl.lightness - 1.0f)) * hsl.saturation;
    float h = hsl.hue / 60.0f;
    float x = c * (1.0f - fabsf(fmodf(h, 2.0f) - 1.0f));
    float m = hsl.lightness - c / 2.0f;
    
    Color color = _hsl_hsv_to_rgb(h, c, x);
    
    return color + m;
}

Color Color::_hsl_hsv_to_rgb(float h, float c, float x) {
    Color color;
    
    if (h >= 0.0f && h < 1.0f) {
        color.red = c;
        color.green = x;
    }
    if (h >= 1.0f && h < 2.0f) {
        color.red = x;
        color.green = c;
    }
    if (h >= 2.0f && h < 3.0f) {
        color.green = c;
        color.blue = x;
    }
    if (h >= 3.0f && h < 4.0f) {
        color.green = x;
        color.blue = c;
    }
    if (h >= 4.0f && h < 5.0f) {
        color.red = x;
        color.blue = c;
    }
    if (h >= 5.0f && h < 6.0f) {
        color.red = c;
        color.blue = x;
    }

    return color;
}

Color Color::operator+(const Color &c) const {
    return Color(red + c.red, green + c.green, blue + c.blue);
}

Color Color::operator-(const Color &c) const {
    return Color(red - c.red, green - c.green, blue - c.blue);
}

Color Color::operator*(const Color &c) const {
    return Color(red * c.red, green * c.green, blue * c.blue);
}

Color Color::operator/(const Color &c) const {
    return Color(red / c.red, green / c.green, blue / c.blue);
}

Color Color::operator+(const float v) const {
    return Color(red + v, green + v, blue + v);
}

Color Color::operator-(const float v) const {
    return Color(red - v, green - v, blue - v);
}

Color Color::operator*(const float v) const {
    return Color(red * v, green * v, blue * v);
}

Color Color::operator/(const float v) const {
    return Color(red / v, green / v, blue / v);
}

Color &Color::operator+=(const Color &c) {
    red += c.red;
    green += c.green;
    blue += c.blue;
    return *this;
}

Color &Color::operator-=(const Color &c) {
    red -= c.red;
    green -= c.green;
    blue -= c.blue;
    return *this;
}

Color &Color::operator*=(const Color &c) {
    red *= c.red;
    green *= c.green;
    blue *= c.blue;
    return *this;
}

Color &Color::operator/=(const Color &c) {
    red /= c.red;
    green /= c.green;
    blue /= c.blue;
    return *this;
}

Color &Color::operator+=(float v) {
    red += v;
    green += v;
    blue += v;
    return *this;
}

Color &Color::operator-=(float v) {
    red -= v;
    green -= v;
    blue -= v;
    return *this;
}

Color &Color::operator*=(float v) {
    red *= v;
    green *= v;
    blue *= v;
    return *this;
}

Color &Color::operator/=(float v) {
    red /= v;
    green /= v;
    blue /= v;
    return *this;
}

Color Color::pow(float exponent) {
    return {std::pow(red, exponent),
            std::pow(green, exponent),
            std::pow(blue, exponent)};
}

void Color::inplace_pow(float exponent) {
    red = std::pow(red, exponent);
    green = std::pow(green, exponent);
    blue = std::pow(blue, exponent);
}