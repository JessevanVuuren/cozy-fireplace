#include <Arduino.h>
#include <Preferences.h>
#include <SSD1351.h>
#include <color.hpp>
#include <graphics.hpp>
#include <noice.hpp>

#define OLED_DC GPIO_NUM_16
#define OLED_CS GPIO_NUM_0
#define OLED_RST GPIO_NUM_4
#define FEED_BUTTON GPIO_NUM_13

#define CLOCK_SPEED 20000000
#define MICRO_IN_SEC 1000000
#define SEC_IN_MICRO 0.000001

constexpr int WIDTH = SSD1351_WIDTH;
constexpr int HEIGHT = SSD1351_HEIGHT;

Vec2 half = Vec2(WIDTH, HEIGHT) / 2;
Vec2 full = Vec2(WIDTH, HEIGHT);

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
constexpr float fall_off = 4.055;

float parabolic(float step, float max, float power) {
    float x = step / max;
    return powf(4.0f * x * (1.0f - x), power);
}

#define SPRINT(s, x) \
    Serial.print(s); \
    Serial.println(x);

void setup() {
    pinMode(FEED_BUTTON, INPUT_PULLDOWN);
    Serial.begin(115200);
    driver.start();

    permutation = make_permutation();

    storage.begin("local");
    contrast_level = storage.getFloat("contrast", 0.0f);
    SPRINT("Contrast: ", contrast_level);
    driver.set_contrast(contrast_level);

    for (int x = 0; x < WIDTH - side_padding * 2; x++) {
        fire_cone[x] = parabolic(x, WIDTH - 1, cone_strength);
    }

    for (int x = 0; x < 256; x++) {
        ColorHSL color = ColorHSL{x / 4.0f, 1.0f, fminf(200, x * 2) / 255.0f};
        fire_colors[x] = Color::to_RGB(color);
    }

    driver.render();
}

inline int surrounding_fire_level(const uint8_t *grid, int x, int y) {
    int fire_1 = grid[((y + 1) % HEIGHT) * WIDTH + ((x - 1 + WIDTH) % WIDTH)];
    int fire_2 = grid[((y + 1) % HEIGHT) * WIDTH + x];
    int fire_3 = grid[((y + 1) % HEIGHT) * WIDTH + ((x + 1) % WIDTH)];
    int fire_4 = grid[((y + 2) % HEIGHT) * WIDTH + x];

    return fire_1 + fire_2 + fire_3 + fire_4;
}

void loop() {

    ulong now = micros();
    ulong dt = now - previous_time;
    previous_time = now;

    for (int x = side_padding; x < WIDTH - side_padding; x++) {
        float heat = 0.0f;
        for (const NoiseLayer &layer : layers) {
            Vec2 norm = Vec2(x, layer.step) / Vec2(WIDTH, HEIGHT);
            Vec2 size = norm * layer.scale;

            uint8_t value = (noise_2D(size, permutation) + 1.0f) * 127.5f;
            heat += value * layer.strength;
        }

        fire_grid[(HEIGHT - 1) * WIDTH + x] = heat * fire_cone[x];
    }

    for (int y = 0; y < HEIGHT - 1; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int heat = surrounding_fire_level(fire_grid, x, y);
            fire_grid[y * WIDTH + x] = heat / fall_off;
        }
    }

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++) {
            Color c = fire_colors[fire_grid[y * WIDTH + x]];
            driver.set_pixel(x, y, c.red, c.green, c.blue);
        }

    for (NoiseLayer &layer : layers)
        layer.step += layer.speed;

    int button_is_pressed = digitalRead(FEED_BUTTON);

    if (button_is_pressed) {

        if (menu_timer == 0) {
            menu_timer = micros() + MICRO_IN_SEC * 2;
            button_was_pressed = true;
        }

        if (micros() > menu_timer) {
            contrast_level += dt * SEC_IN_MICRO;
            if (contrast_level >= 16.0f)
                contrast_level = 0.0f;

            driver.set_contrast(contrast_level);

            Vec2 start = Vec2(0, HEIGHT / 2 - HEIGHT * 0.1);
            Vec2 stop = Vec2(WIDTH, HEIGHT / 2 + HEIGHT * 0.1);

            char text_buf[32];
            snprintf(text_buf, sizeof(text_buf), "Contrast %d", (int)contrast_level);

            canvas.square(start, stop, Color::Black);
            canvas.set_text(Vec2(30, HEIGHT / 2), Color::White, text_buf);
        }
    } else {
        if (button_was_pressed) {
            storage.putFloat("contrast", contrast_level);
            button_was_pressed = false;
        }
        menu_timer = 0;
    }

    driver.render();

    SPRINT("FPS: ", MICRO_IN_SEC / (micros() - now));
}