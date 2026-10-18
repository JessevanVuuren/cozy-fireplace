#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <cstdint>
#include <string>

typedef struct {
    float hue;
    float saturation;
    float value;
} ColorHSV;

typedef struct {
    float hue;
    float saturation;
    float lightness;
} ColorHSL;

class Color {

  public:
    float red;
    float green;
    float blue;

    Color();
    Color(float value);
    Color(float red, float green, float blue);

    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color White;
    static const Color Black;

    Color &to_sRGB();
    Color &to_gamma();
    Color &to_8bit();
    Color &to_16bit();
    Color &to_18bit();

    const char *to_string() const;

    static Color to_RGB(ColorHSV hsv);
    static Color to_RGB(ColorHSL hsl);

    Color operator+(const Color &c) const;
    Color operator-(const Color &c) const;
    Color operator*(const Color &c) const;
    Color operator/(const Color &c) const;

    Color operator+(const float v) const;
    Color operator-(const float v) const;
    Color operator*(const float v) const;
    Color operator/(const float v) const;

    Color &operator+=(const Color &c);
    Color &operator-=(const Color &c);
    Color &operator*=(const Color &c);
    Color &operator/=(const Color &c);

    Color &operator+=(float c);
    Color &operator-=(float c);
    Color &operator*=(float c);
    Color &operator/=(float c);

    Color pow(float exponent);
    void inplace_pow(float exponent);

  private:
    float _convert_sRGB(float value) const;
    static Color _hsl_hsv_to_rgb(float h, float c, float x);
};

#endif // COLOR_H
