# KeyboardMF Fork

This is a fork of the [KeyboardMF project](https://github.com/VolosR/KeyboardMF/tree/main) ported to PlatformIO with updated LVGL configuration. The original project was already designed for ESP32-S3, and this fork focuses on:

1. Migration to PlatformIO build system
2. Integration of LVGL v9.2.2 configuration
3. Improved project structure following PlatformIO conventions

## Hardware Requirements

This project is designed for the [Makerfabs ESP32-S3 Parallel TFT with Touch 7"](https://www.makerfabs.com/esp32-s3-parallel-tft-with-touch-7-inch.html) board, which features:

- ESP32-S3-WROOM-1 with 16MB Flash and 8MB PSRAM
- 7-inch IPS display (1024x600 or 800x480 resolution)
- GT911 capacitive touch controller (5-point touch)
- Dual USB Type-C ports (one for USB-to-UART, one for native USB)
- MicroSD card slot
- 2x Mabee/Grove connectors (I2C and GPIO)

## Project Setup

This project uses PlatformIO for development. The main configurations include:

- ESP32-S3 platform with Arduino framework
- 16MB flash configuration
- PSRAM support enabled
- USB Host capabilities
- LVGL v9.2.2 configuration included in `include/lv_conf.h`

### Dependencies

- LovyanGFX (v1.1.9) - Graphics library
- NeoPixelBus (v2.7.6) - LED control
- LVGL (v9.2.2) - UI framework
- EspUsbHost (v1.0.2) - USB Host functionality

## Development

The project is structured using PlatformIO's standard layout:

- `/src` - Main source code
- `/include` - Header files (including LVGL configuration)
- `/lib` - Project-specific libraries
- `/test` - Test files

## Building and Flashing

1. Open the project in PlatformIO
2. Build using the PlatformIO build command
3. Upload to your ESP32-S3 board using either USB port (USB-to-UART or native USB)

## License

This project maintains the same license as the original KeyboardMF project.
