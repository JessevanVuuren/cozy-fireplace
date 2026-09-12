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
#define FEED_BUTTON GPIO_NUM_13
#define CLOCK_SPEED 20000000 // 20 MHz

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_16);

const uint8_t width = SSD1351_WIDTH;
const uint8_t height = SSD1351_HEIGHT;

const int noise_height = height * 2;
const float noise_scale = 4.3;
const float noise_speed = 0.3;

int *perm;

uint16_t palette[256];
uint16_t fire[height * width];

typedef struct {
    float scale;
    float speed;
    float step;
    float strength;
} Noise;

Noise layers[] = {
    {4.5, 0.2, 0.0, 0.45},
    {15.4, 0.3, 0.0, 0.15},
    {1.3, 1.3, 0.0, 0.4}};

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    Serial.begin(115200);
    driver.start();

    perm = make_permutation();

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        palette[x] = Color::to_RGB(color).to_16bit();
    }
}

int surrounding_fire_level(uint16_t *fire, int x, int y, int width, int height) {
    int fire_1 = fire[((y + 1) % height) * width + ((x - 1 + width) % width)];
    int fire_2 = fire[((y + 1) % height) * width + x];
    int fire_3 = fire[((y + 1) % height) * width + ((x + 1) % width)];
    int fire_4 = fire[((y + 2) % height) * width + x];

    return fire_1 + fire_2 + fire_3 + fire_4;
}

void loop() {

    unsigned long time = micros();

    for (int x = 0; x < width; x++) {
        float n = 0.0f;
        for (const Noise &layer : layers) {
            Vec2 pos = Vec2(x, layer.step) / Vec2(width, height);
            Vec2 height = pos * layer.scale;

            uint8_t l = (noise_2D(height, perm) + 1.0f) * 127.5f;
            n += l * layer.strength;
        }

        float half = width / 2;
        float norm = 1.0;
        if (x < half) {
            norm = x / half;
        } else {
            norm = x-half / width;
            norm = 1.0 - norm;
        }

        fire[(height - 1) * width + x] = n * norm;
    }

    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width; x++) {
            int heat = surrounding_fire_level(fire, x, y, width, height);
            fire[y * width + x] = heat / 4.055f;
        }
    }

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            driver.set_pixel(x, y, palette[fire[y * width + x]]);

    for (Noise &layer : layers)
        layer.step += layer.speed;

    int feed_fire = digitalRead(FEED_BUTTON);

    driver.render();
    // Serial.print("FEED: ");
    // Serial.print(feed_fire);
    // Serial.print(", FPS: ");
    // Serial.println(1000000.0 / (micros() - time));
}