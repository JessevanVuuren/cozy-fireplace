#include <Arduino.h>
#include <SSD1351.h>
#include <color.h>
#include <graphics.h>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 32768 bytes = 32KB

#define OLED_DC GPIO_NUM_2
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define CLOCK_SPEED 20000000 // 20 MHz

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_16);
Graphics canvas(SSD1351_WIDTH, SSD1351_HEIGHT);

uint8_t width = SSD1351_WIDTH;
uint8_t height = SSD1351_HEIGHT;

void setup() {
    Serial.begin(115200);
    driver.start(canvas.get_buffer(), canvas.buffer_size());
}

void loop() {
    ColorHSV hsv = {(sin(millis() * 0.001f) + 1.0f) * 0.5f * 360.0f, 1.0f, 1.0f};
    Color rgb = Color::to_RGB(hsv);
    Serial.println(hsv.hue);
    driver.fill_buffer(rgb.to_16bit());

    driver.render();
}

// pio run --target upload
// pio device monitor
