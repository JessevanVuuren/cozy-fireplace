#include <Arduino.h>
#include <SSD1351.h>
#include <color.h>
#include <graphics.h>
#include <noice.h>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 128 * 128 = 16384(utf8) bytes
// 16384 * 2 for 16bit color = 32768 bytes = 32KB
// fasted time = FPS: 67.76

#define OLED_DC GPIO_NUM_16
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4

#define FEED_BUTTON GPIO_NUM_13

#define CLOCK_SPEED 20000000 // 20 MHz

static constexpr int width = SSD1351_WIDTH;
static constexpr int height = SSD1351_HEIGHT;

Vec2 half = Vec2(width, height) / 2;

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_18);
Graphics canvas(width, height, [](int x, int y, int c) {
    driver.set_pixel(x, y, c);
});

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    Serial.begin(115200);
    driver.start();
}

void debug_fps(unsigned long time_us) {
    Serial.print(", FPS: ");
    Serial.println(1000000.0 / (micros() - time_us));
}

void loop() {
    unsigned long time = micros();

    int feed_fire = digitalRead(FEED_BUTTON);

    canvas.circle(half, 10, Color::Blue);

    driver.render();

    debug_fps(time);
}