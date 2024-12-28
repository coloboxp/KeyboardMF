# KeyboardMF Fork

This is a fork of the [KeyboardMF project](https://github.com/VolosR/KeyboardMF/tree/main) ported to PlatformIO with updated LVGL configuration. The original project was already designed for ESP32-S3, and this fork focuses on:

1. Migration to PlatformIO build system
2. Integration of LVGL v9.2.2 configuration
3. Improved project structure following PlatformIO conventions
4. Comprehensive documentation and diagrams

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

- LovyanGFX (v1.1.9) - Graphics library for display handling
- NeoPixelBus (v2.7.6) - LED control and effects
- LVGL (v9.2.2) - Modern UI framework
- EspUsbHost (v1.0.2) - USB Host functionality for keyboard input

## Project Structure

The project follows PlatformIO's standard layout with additional documentation:

```
KeyboardMF/
├── src/                    # Main source code
│   ├── KeyboardManager.cpp # USB keyboard handling
│   ├── Display.cpp        # Display operations
│   ├── TextEditor.cpp     # Text editing functionality
│   ├── FileManager.cpp    # File operations
│   └── NeoPixelManager.cpp# LED control
├── include/               # Header files
│   ├── KeyboardConfig.hpp # Configuration constants
│   ├── lv_conf.h         # LVGL configuration
│   └── *.hpp             # Component headers
├── doc/                   # Documentation
│   └── system_diagrams.md # Class and sequence diagrams
├── lib/                   # Project-specific libraries
├── test/                  # Test files
└── platformio.ini         # PlatformIO configuration
```

## Building and Flashing

1. Install PlatformIO IDE (VS Code extension) or CLI
2. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/KeyboardMF.git
   cd KeyboardMF
   ```
3. Build the project:
   ```bash
   pio run
   ```
4. Upload to your ESP32-S3 board:
   ```bash
   pio run --target upload
   ```

## Documentation

- Complete system documentation with class and sequence diagrams is available in the `doc/` directory
- The diagrams are in Mermaid format and can be viewed directly on GitHub
- Component interactions and dependencies are fully documented

## Changelog

### [2024-12-27] - Major Refactor: PlatformIO Port

- Migrated build system to PlatformIO
- Updated to LVGL v9.2.2
- Restructured project following PlatformIO conventions
- Added comprehensive system documentation and diagrams
- Improved code organization with proper namespacing
- Enhanced error handling and initialization sequences

### [2024-12-27] - Initial Fork

- Created fork from original KeyboardMF project
- Initial files upload
- Base ESP32-S3 configuration

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'feat: add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project maintains the same license as the original KeyboardMF project.

## Acknowledgments

- Original [KeyboardMF project](https://github.com/VolosR/KeyboardMF/tree/main) by VolosR
- [Makerfabs](https://www.makerfabs.com/) for the ESP32-S3 hardware
- All contributors to the dependent libraries
