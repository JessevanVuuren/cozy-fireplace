#include <Arduino.h>
#include <SSD1351.h>
#include <color.h>
#include <graphics.h>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 128 * 128 = 16384(utf8) bytes
// 16384 * 2 for 16bit color = 32768 bytes = 32KB
// fasted time = FPS: 67.76

#define OLED_DC GPIO_NUM_2
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define CLOCK_SPEED 20000000 // 20 MHz

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_16);

setPixelFunc pixel = [](int x, int y, uint32_t color) {
    driver.set_pixel(x, y, color);
};

Graphics canvas(SSD1351_WIDTH, SSD1351_HEIGHT, pixel);

uint8_t width = SSD1351_WIDTH;
uint8_t height = SSD1351_HEIGHT;

void setup() {
    Serial.begin(115200);
    driver.start();
}

void loop() {



    driver.render();
}

// pio run --target upload
// pio device monitor
