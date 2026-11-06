#include <Arduino.h>
#include <Preferences.h>
#include <SSD1351.h>
#include <color.hpp>
#include <graphics.hpp>
#include <map>
#include <noice.hpp>

#define OLED_DC GPIO_NUM_16
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define FEED_BUTTON GPIO_NUM_13

#define CLOCK_SPEED 20000000
#define MICRO_IN_SEC 1000000
#define SEC_IN_MICRO 0.000001

#define SPRINT_V(s, x) \
    Serial.print(s);   \
    Serial.println(x);

#define SPRINT(s) \
    Serial.println(s);

constexpr int WIDTH = SSD1351_WIDTH;
constexpr int HEIGHT = SSD1351_HEIGHT;

Vec2 half = Vec2(WIDTH, HEIGHT) / 2;
Vec2 full = Vec2(WIDTH, HEIGHT);
Vec2 over_full = 1.0f / Vec2(WIDTH, HEIGHT);

SSD1351 driver(OLED_CS, OLED_DC, OLED_RST, CLOCK_SPEED, ColorLevel::BIT_18);
Graphics canvas(WIDTH, HEIGHT, [](int x, int y, Color c) {
    driver.set_pixel(x, y, c.red, c.green, c.blue);
});

Preferences storage;

int *permutation = nullptr;
float fire_cone[WIDTH] = {0.0};

uint32_t menu_timer = 0;
uint32_t previous_time = 0;
float contrast_level = 0.0;
bool button_was_pressed = false;

Color fire_colors[256];
uint8_t fire_grid[HEIGHT * WIDTH] = {0};

typedef struct {
    float scale;
    float speed;
    float step;
    float strength;
} NoiseLayer;

// Fire settings:
NoiseLayer layers[] = {
    {4.5, 0.2, 0.0, 0.45},
    {15.4, 0.3, 0.0, 0.15},
    {1.3, 1.3, 0.0, 0.4}};

constexpr float side_padding = 2.0;
constexpr float cone_strength = 0.5;
constexpr float over_fall_off = 1.0f / 4.055;

float parabolic(float step, float max, float power) {
    float x = step / max;
    return powf(4.0f * x * (1.0f - x), power);
}

inline int surrounding_fire_level(const uint8_t *grid, int x, int y) {
    return grid[((y + 1) & (HEIGHT - 1)) * WIDTH + ((x - 1 + WIDTH) & (WIDTH - 1))] +
           grid[((y + 1) & (HEIGHT - 1)) * WIDTH + x] +
           grid[((y + 1) & (HEIGHT - 1)) * WIDTH + ((x + 1) & (WIDTH - 1))] +
           grid[((y + 2) & (HEIGHT - 1)) * WIDTH + x];
}

std::map<std::string, std::vector<ulong>> performance;

void print_performance() {
    SPRINT("\nPerformance: ")
    float total = 0.0f;
    for (const auto &record : performance) {
        ulong local = 0;

        for (const ulong value : record.second) {
            local += value;
        }

        float avg = local / record.second.size();
        total += avg;

        std::string label = record.first + ": ";
        SPRINT_V(label.c_str(), avg)
    }
    SPRINT_V("TOTAL: ", total / performance.size());
}

void speed() {
    ulong now = micros();

    ulong start = micros();
    for (int x = side_padding; x < WIDTH - side_padding; x++) {

        float heat = 0.0f;
        for (const NoiseLayer &layer : layers) {
            Vec2 norm = Vec2(x, layer.step) * over_full * layer.scale;

            uint8_t value = (noise_2D(norm, permutation) + 1.0f) * 127.5f;
            heat += value * layer.strength;
        }

        fire_grid[(HEIGHT - 1) * WIDTH + x] = heat * fire_cone[x];
    }

    performance["layer"].push_back(micros() - start);

    start = micros();
    for (int y = 0; y < HEIGHT - 1; y++) {
        int dy = y * WIDTH;
        for (int x = 0; x < WIDTH; x++) {
            int heat = surrounding_fire_level(fire_grid, x, y);
            fire_grid[dy + x] = heat * over_fall_off;
        }
    }

    performance["surrounding"].push_back(micros() - start);

    start = micros();
    for (int y = 0; y < HEIGHT; y++) {
        int dy = y * WIDTH;
        for (int x = 0; x < WIDTH; x++) {
            uint8_t heat_index = fire_grid[dy + x];
            if (heat_index) {
                Color c = fire_colors[heat_index];
                driver.set_pixel(x, y, c.red, c.green, c.blue);
            }
        }
    }

    performance["set_pixel"].push_back(micros() - start);

    start = micros();

    for (NoiseLayer &layer : layers)
        layer.step += layer.speed;

    performance["step"].push_back(micros() - start);

    start = micros();
    driver.render();
    performance["render"].push_back(micros() - start);

    SPRINT_V("FPS: ", MICRO_IN_SEC / (micros() - now));
}

void setup() {
    driver.start();
    Serial.begin(115200);
    storage.begin("local");
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);

    contrast_level = storage.getFloat("contrast", 0.0f);
    SPRINT_V("Contrast: ", contrast_level);
    driver.set_contrast(contrast_level);

    permutation = make_permutation();

    for (int x = 0; x < WIDTH - side_padding * 2; x++) {
        fire_cone[x] = parabolic(x, WIDTH - 1, cone_strength);
    }

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        fire_colors[x] = Color::to_RGB(color);
    }

    driver.render();

    for (int i = 0; i < 250; i++) {
        speed();
    }

    print_performance();
}

void loop() {
}

// Performance:
// layer: 1559.00
// render: 22117.00
// set_pixel: 7508.00
// step: 2.00
// surrounding: 7448.00
// TOTAL: 7726.80
// FPS: 25

// Performance:
// layer: 1559.00
// render: 22117.00
// set_pixel: 7508.00
// step: 2.00
// surrounding: 4513.00
// TOTAL: 7139.80
// FPS: 27

// Performance:
// layer: 1399.00
// render: 22125.00
// set_pixel: 7508.00
// step: 2.00
// surrounding: 4517.00
// TOTAL: 7110.20
// FPS: 28

// Performance:
// layer: 1399.00
// render: 22117.00
// set_pixel: 5009.00
// step: 2.00
// surrounding: 4513.00
// TOTAL: 6608.00
// FPS: 29 / 33

// Performance:
// layer: 1399.00
// render: 22117.00
// set_pixel: 4872.00
// step: 2.00
// surrounding: 4513.00
// TOTAL: 6580.60
// FPS: 30

// Performance:
// layer: 1399.00
// render: 22117.00
// set_pixel: 4872.00
// step: 2.00
// surrounding: 4377.00
// TOTAL: 6553.40
// FPS: 30