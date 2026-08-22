// https://github.com/Gecko05/SSD1351-Driver-Library/blob/master/src/ssd1351.h
// https://www.waveshare.com/wiki/1.5inch_RGB_OLED_Module#Arduino_Tutorial
// https://files.waveshare.com/upload/a/a7/SSD1351-Revision_1.5.pdf

#ifndef SSD1351_H
#define SSD1351_H

#include <SPI.h>
#include <cstdint>

#define SSD1351_SET_COLUMN_ADDRESS 0x15
#define SSD1351_SET_ROW_ADDRESS 0x75
#define SSD1351_WRITE_RAM 0x5C
#define SSD1351_READ_RAM 0x5D
#define SSD1351_SET_REMAP_COLOR 0xA0
#define SSD1351_START_LINE 0xA1
#define SSD1351_DISPLAY_OFFSET 0xA2
#define SSD1351_DISPLAY_ALL_OFF 0xA4
#define SSD1351_DISPLAY_ALL_ON 0xA5
#define SSD1351_NORMAL_DISPLAY 0xA6
#define SSD1351_INVERT_DISPLAY 0xA7
#define SSD1351_FUNCTION_SELECT 0xAB
#define SSD1351_DISPLAY_OFF 0xAE
#define SSD1351_DISPLAY_ON 0xAF
#define SSD1351_PRE_CHARGE 0xB1
#define SSD1351_DISPLAY_ENHANCE 0xB2
#define SSD1351_CLOCK_DIV 0xB3
#define SSD1351_SET_VSL 0xB4
#define SSD1351_SET_GPIO 0xB5
#define SSD1351_PRE_CHARGE_2 0xB6
#define SSD1351_SET_GRAY 0xB8
#define SSD1351_USE_LUT 0xB9
#define SSD1351_PRE_CHARGE_LEVEL 0xBB
#define SSD1351_VCOMH 0xBE
#define SSD1351_CONTRAST_ABC 0xC1
#define SSD1351_CONTRAST_MASTER 0xC7
#define SSD1351_MUX_RATIO 0xCA
#define SSD1351_COMMAND_LOCK 0xFD
#define SSD1351_UNLOCK_DRIVER 0x12
#define SSD1351_HORIZ_SCROLL 0x96
#define SSD1351_STOP_SCROLL 0x9E
#define SSD1351_START_SCROLL 0x9F

#define SSD1351_ZERO 0
#define SSD1351_WIDTH 128  // 128 pixels in total
#define SSD1351_HEIGHT 128 // 128 pixels in total

#define SSD1351_PIXELS (SSD1351_HEIGHT * SSD1351_WIDTH)

// colors
#define COLOR_WHITE 0xFFFF
#define COLOR_BLACK 0x0000
#define COLOR_BLUE 0x001F
#define COLOR_RED 0xF800
#define COLOR_MAGENTA 0xF81F
#define COLOR_GREEN 0x07E0
#define COLOR_CYAN 0x7FFF
#define COLOR_YELLOW 0xFFE0
#define COLOR_BROWN 0XBC40
#define COLOR_GRAY 0X8430

enum ColorLevel {
    BIT_8,
    BIT_16,
    BIT_18
};

class SSD1351 {
  public:
    SSD1351(int cs, int dc, int rst, unsigned long hz, ColorLevel color_bit);
    ~SSD1351();

    void reset();
    void init_SPI();
    void init_gpio_pins();
    void init_oled_regi();

    void display_on();
    void display_off();

    void start();
    void start(uint8_t *buffer, int buffer_size);

    void write_regi(int);
    void write_data(int);

    void clear_buffer();
    void create_buffer();
    void fill_buffer(uint32_t color);

    void set_pixel(int x, int y, uint32_t color);
    uint32_t get_pixel(int x, int y) const;

    void render();

  private:
    int _hz;
    int _cs;
    int _dc;
    int _rst;

    uint8_t *_buffer;
    int _buffer_size;
    bool _owns_buffer = true;

    ColorLevel _color_bit;

    int _get_buffer_size();

    void _start();
};

#endif // SSD1351_H
