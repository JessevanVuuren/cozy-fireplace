#include <Arduino.h>
#include <SSD1351.h>
#include <color.h>
#include <graphics.h>
#include <noice.h>

// HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
// buffer = 128 * 128 = 16384(utf8) bytes
// 16384 * 2 for 16bit color = 32768 bytes = 32KB
// fasted time = FPS: 67.76

#define OLED_DC GPIO_NUM_2
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define CLOCK_SPEED 20000000 // 20 MHz

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_16);

const uint8_t width = SSD1351_WIDTH;
const uint8_t height = SSD1351_HEIGHT;

const int noise_height = height * 2;
const float noise_scale = 4.3;

int *perm = make_permutation();

uint8_t noise_map[noise_height][width];

uint32_t palette[256];
uint32_t fire[height][width];

void setup() {
    Serial.begin(115200);
    driver.start();

    for (int y = 0; y < noise_height; y++) {
        for (int x = 0; x < width; x++) {
            Vec2 point = (Vec2(x, y) / Vec2(width, height)) * noise_scale;
            uint8_t n = (noise_2D(point, perm) + 1.0f) * 127.5f;

            noise_map[y][x] = n;
        }
    }

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        palette[x] = Color::to_RGB(color).to_16bit();
    }
}

float count = 0;
void loop() {

    unsigned long time = micros();

    for (int x = 0; x < width; x++) {
        uint8_t n = noise_map[(int)count % noise_height][x];
        fire[height - 1][x] = n;
        // driver.set_pixel(x, height - 1, n);
    }

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
    count += 1.0f * 0.3f;

    Serial.print("FPS: ");
    Serial.println(1000000.0 / (micros() - time));
}

// pio run --target upload
// pio device monitor
