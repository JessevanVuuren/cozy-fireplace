#include <Arduino.h>
#include <Preferences.h>
#include <SSD1351.h>
#include <color.hpp>
#include <graphics.hpp>
#include <noice.hpp>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 128 * 128 = 16384(utf8) bytes
// 16384 * 2 for 16bit color = 32768 bytes = 32KB
// fasted time = FPS: 67.76

#define OLED_DC GPIO_NUM_16
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4

#define FEED_BUTTON GPIO_NUM_13

#define CLOCK_SPEED 20000000 // 20 MHz
#define MICRO_IN_SEC 1000000
#define SEC_IN_MICRO 0.000001

#define SIDE_PADDING 2
#define CONE_STRENGTH 0.5

constexpr uint8_t width = SSD1351_WIDTH;
constexpr uint8_t height = SSD1351_HEIGHT;

Vec2 half = Vec2(width, height) / 2;

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_18);
Graphics canvas(width, height, [](int x, int y, Color c) {
    driver.set_pixel(x, y, c.to_18bit());
});

Rectangle rect = Rectangle(half, Vec2(40, 40), Color::Red);
Circle cir = Circle(half, 20.0f, Color::Red);

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    Serial.begin(115200);
    driver.start();

    // canvas.add_object(rect);
    canvas.add_object(cir);
}

void loop() {
    canvas.draw_objects();

    // rect.set_angle(sin(micros() * SEC_IN_MICRO) * PI);
    // rect.set_color(Color::to_RGB(ColorHSV{(sin(micros() * SEC_IN_MICRO) + 1.0f) * 0.5f * 360.0f, 1.0f, 1.0f}));
    cir.set_color(Color::to_RGB(ColorHSV{(sin(micros() * SEC_IN_MICRO) + 1.0f) * 0.5f * 360.0f, 1.0f, 1.0f}));

    driver.render();
}