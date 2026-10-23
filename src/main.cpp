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
Vec2 full = Vec2(width, height);

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_18);
Graphics canvas(width, height, [](int x, int y, Color c) {
    driver.set_pixel(x, y, c.red, c.green, c.blue);
});

Circle cir = Circle(half, 20.0f, Color::Red);

Rectangle rect = Rectangle(Vec2(width / 3.0f * 1.0f, half.y), Vec2(30, 30), Color::Green);
Bitmap bit = Bitmap(Vec2(width / 3.0f * 2.0f, half.y), 30, 30);

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    Serial.begin(115200);
    driver.start();

    for (int y = 0; y < bit.get_height(); y++) {
        for (int x = 0; x < bit.get_width(); x++) {
            Vec2 uv = Vec2(x, y) / bit.get_size();
            Color q = Color(uv.x, uv.y, 0.0);
            bit.set_pixel(x, y, q.to_8bit());
        }
    }

    canvas.add_object(bit);
    canvas.add_object(rect);
}

void loop() {
    unsigned long time = micros();

    canvas.draw_objects();

    bit.set_angle(sin(time * 0.000001f) * PI);
    rect.set_angle(sin(time * 0.000001f) * PI);

    driver.render();
}