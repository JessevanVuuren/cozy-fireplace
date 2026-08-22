#include <Arduino.h>
#include <SSD1351.h>
#include <color.h>
#include <graphics.h>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 128 * 128 = 16384(utf8) bytes
// 16384 * 2 for 16bit color = 32768 bytes = 32KB
// fasted time = FPS: 67.76

// https://lodev.org/cgtutor/fire.html#The_Classic_Fire_Effect

#define OLED_DC GPIO_NUM_2
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define CLOCK_SPEED 20000000 // 20 MHz

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_16);

setPixelFunc pixel = [](int x, int y, uint32_t color) {
    driver.set_pixel(x, y, color);
};

Graphics canvas(SSD1351_WIDTH, SSD1351_HEIGHT, pixel);

const uint8_t width = SSD1351_WIDTH;
const uint8_t height = SSD1351_HEIGHT;

uint32_t palette[256];
uint32_t fire[height][width];

void setup() {
    Serial.begin(115200);
    driver.start();

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        palette[x] = Color::to_RGB(color).to_16bit();
    }
    for (int x = 0; x < width; x++) {
        fire[height - 1][x] = abs(32768 + rand()) % 256;
    }
}

void loop() {


    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width; x++) {
            int fire_1 = fire[(y + 1) % height][(x - 1 + width) % width];
            int fire_2 = fire[(y + 1) % height][(x) % width];
            int fire_3 = fire[(y + 1) % height][(x + 1) % width];
            int fire_4 = fire[(y + 2) % height][(x) % width];

            fire[y][x] = (fire_1 + fire_2 + fire_3 + fire_4) / 4.055f;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            driver.set_pixel(x, y, palette[fire[y][x]]);

    driver.render();
}

// pio run --target upload
// pio device monitor
