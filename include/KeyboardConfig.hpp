#pragma once

namespace keyboard
{
    namespace config
    {

        // Display configuration
        static constexpr int SCREEN_WIDTH = 1024;
        static constexpr int SCREEN_HEIGHT = 600;
        static constexpr int DEFAULT_BRIGHTNESS_INDEX = 2;
        static constexpr int BRIGHTNESS_LEVELS[] = {50, 90, 120, 160, 200, 250};

        // NeoPixel configuration
        static constexpr int NEOPIXEL_COUNT = 8;
        static constexpr int NEOPIXEL_PIN = 18;

        // File System Configuration
        static constexpr int MAX_FILES = 32;
        static constexpr int MAX_LINE_LENGTH = 256;
        static constexpr int MAX_LINES = 16;

        // Pin Definitions
        static constexpr int PIN_SD_CMD = 11;
        static constexpr int PIN_SD_CLK = 12;
        static constexpr int PIN_SD_D0 = 13;

    } // namespace config
} // namespace keyboard