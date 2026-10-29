## Hardware
- ESP32
- SSD1351

## Pictures

<img width="606" height="470" alt="Image" src="https://github.com/user-attachments/assets/bc32615c-3368-4bb9-9abf-7fda3a40be76" />

<img width="606" height="463" alt="Image" src="https://github.com/user-attachments/assets/a111bbd3-9a40-4a27-b3f3-23e661e16047" />

## Links

board:
- https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/

libs:
- https://github.com/Gecko05/SSD1351-Driver-Library/blob/master/src/ssd1351.h

display:
- https://www.waveshare.com/wiki/1.5inch_RGB_OLED_Module#Arduino_Tutorial
- https://files.waveshare.com/upload/a/a7/SSD1351-Revision_1.5.pdf

font:
- https://github.com/trip5/Matrix-Fonts/blob/main/8-series/MatrixLight8x6.bdf

commands: 
- pio run --target upload
- pio device monitor

todo:
- support alpha
