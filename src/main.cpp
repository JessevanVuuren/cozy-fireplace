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
Graphics canvas(width, height, [](int x, int y, int c) {
    driver.set_pixel(x, y, c);
});

Preferences storage;

int *permutation;
float fire_cone[width - 1] = {0};

volatile float contrast_level = 0;

volatile ulong menu = 0;
volatile ulong previous_time = 0;

uint32_t palette[256];
uint32_t fire[height * width];

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

float parabolic(float step, float max, float power) {
    float x = step / max;
    return powf(4.0f * x * (1 - x), power);
}

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    storage.begin("local");
    Serial.begin(115200);
    driver.start();

    permutation = make_permutation();

    contrast_level = storage.getFloat("contrast-level", 0);
    driver.set_contrast(contrast_level);

    for (int x = 0; x < width - SIDE_PADDING * 2; x++) {
        fire_cone[x] = parabolic(x, width - 1, CONE_STRENGTH);
    }

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        palette[x] = Color::to_RGB(color).to_18bit();
    }

    driver.render();
}

int surrounding_fire_level(size_t *fire, int x, int y, int width, int height) {
    int fire_1 = fire[((y + 1) % height) * width + ((x - 1 + width) % width)];
    int fire_2 = fire[((y + 1) % height) * width + x];
    int fire_3 = fire[((y + 1) % height) * width + ((x + 1) % width)];
    int fire_4 = fire[((y + 2) % height) * width + x];

    return fire_1 + fire_2 + fire_3 + fire_4;
}

void debug_fps(unsigned long time_us) {
    Serial.print("FPS: ");
    Serial.println(1000000.0f / (micros() - time_us));
}

void loop() {

    ulong now = micros();
    ulong dt = now - previous_time;
    previous_time = now;

    for (int x = SIDE_PADDING; x < width - SIDE_PADDING; x++) {
        float heat = 0.0f;
        for (const Noise &layer : layers) {
            Vec2 norm = Vec2(x, layer.step) / Vec2(width, height);
            Vec2 size = norm * layer.scale;

            uint8_t value = (noise_2D(size, permutation) + 1.0f) * 127.5f;
            heat += value * layer.strength;
        }

        fire[(height - 1) * width + x] = heat * fire_cone[x];
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

    if (feed_fire) {

        if (menu == 0) {
            menu = now + MICRO_IN_SEC * 2;
        }

        if (now > menu) {
            contrast_level += dt * SEC_IN_MICRO;
            if (contrast_level >= 16.0f) {
                contrast_level -= 16.0f;
            }

            driver.set_contrast((int)contrast_level);
            storage.putFloat("contrast", contrast_level);

            Vec2 start = Vec2(0, height / 2 - height * 0.1);
            Vec2 stop = Vec2(width, height / 2 + height * 0.1);

            canvas.square(start, stop, Color::Black);
            canvas.set_text(Vec2(30, height / 2), Color::White, "Contrast " + String((int)contrast_level));
        }
    } else {
        menu = 0;
    }

    driver.render();

    // debug_fps(time);
}