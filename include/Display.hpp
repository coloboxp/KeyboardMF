#pragma once

#define LGFX_USE_V1
#define LGFX_MAKERFABS_MATOUCH_1024X768

#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>
#include "KeyboardConfig.hpp"

namespace keyboard
{

    class LGFX : public lgfx::LGFX_Device
    {
    public:
        lgfx::Bus_RGB _bus_instance;
        lgfx::Panel_RGB _panel_instance;
        lgfx::Light_PWM _light_instance;
        lgfx::Touch_GT911 _touch_instance;

        LGFX(void)
        {
            {
                auto cfg = _panel_instance.config();
                cfg.memory_width = config::SCREEN_WIDTH;
                cfg.panel_width = config::SCREEN_WIDTH;
                cfg.memory_height = config::SCREEN_HEIGHT;
                cfg.panel_height = config::SCREEN_HEIGHT;
                cfg.offset_x = 0;
                cfg.offset_y = 0;
                _panel_instance.config(cfg);
            }

            {
                auto cfg = _panel_instance.config_detail();
                cfg.use_psram = 1;
                _panel_instance.config_detail(cfg);
            }

            {
                auto cfg = _bus_instance.config();
                cfg.panel = &_panel_instance;
                cfg.pin_d0 = GPIO_NUM_8;   // B0
                cfg.pin_d1 = GPIO_NUM_3;   // B1
                cfg.pin_d2 = GPIO_NUM_46;  // B2
                cfg.pin_d3 = GPIO_NUM_9;   // B3
                cfg.pin_d4 = GPIO_NUM_1;   // B4
                cfg.pin_d5 = GPIO_NUM_5;   // G0
                cfg.pin_d6 = GPIO_NUM_6;   // G1
                cfg.pin_d7 = GPIO_NUM_7;   // G2
                cfg.pin_d8 = GPIO_NUM_15;  // G3
                cfg.pin_d9 = GPIO_NUM_16;  // G4
                cfg.pin_d10 = GPIO_NUM_4;  // G5
                cfg.pin_d11 = GPIO_NUM_45; // R0
                cfg.pin_d12 = GPIO_NUM_48; // R1
                cfg.pin_d13 = GPIO_NUM_47; // R2
                cfg.pin_d14 = GPIO_NUM_21; // R3
                cfg.pin_d15 = GPIO_NUM_14; // R4

                cfg.pin_henable = GPIO_NUM_40;
                cfg.pin_vsync = GPIO_NUM_41;
                cfg.pin_hsync = GPIO_NUM_39;
                cfg.pin_pclk = GPIO_NUM_42;
                cfg.freq_write = 16000000;

                cfg.hsync_polarity = 0;
                cfg.hsync_front_porch = 80;
                cfg.hsync_pulse_width = 4;
                cfg.hsync_back_porch = 16;
                cfg.vsync_polarity = 0;
                cfg.vsync_front_porch = 22;
                cfg.vsync_pulse_width = 4;
                cfg.vsync_back_porch = 4;
                cfg.pclk_idle_high = 1;
                _bus_instance.config(cfg);
            }
            _panel_instance.setBus(&_bus_instance);

            {
                auto cfg = _light_instance.config();
                cfg.pin_bl = GPIO_NUM_10;
                cfg.invert = true;
                _light_instance.config(cfg);
            }
            _panel_instance.light(&_light_instance);

            {
                auto cfg = _touch_instance.config();
                cfg.x_min = 0;
                cfg.y_min = 0;
                cfg.x_max = config::SCREEN_WIDTH;
                cfg.y_max = config::SCREEN_HEIGHT;
                cfg.bus_shared = false;
                cfg.offset_rotation = 0;
                cfg.i2c_port = I2C_NUM_1;
                cfg.pin_sda = GPIO_NUM_17;
                cfg.pin_scl = GPIO_NUM_18;
                cfg.pin_int = GPIO_NUM_NC;
                cfg.pin_rst = GPIO_NUM_38;
                cfg.freq = 400000;
                _touch_instance.config(cfg);
                _panel_instance.setTouch(&_touch_instance);
            }

            setPanel(&_panel_instance);
        }
    };

    class Display
    {
    public:
        Display();
        ~Display() = default;

        void init();
        void setBrightness(uint8_t level);
        void cycleBrightness();
        void drawText(const std::string &text, int x, int y, int size = 1, uint16_t color = 0xFFFF);
        void drawLine(int x1, int y1, int x2, int y2, uint16_t color);
        void fillRect(int x, int y, int w, int h, uint16_t color);
        void clearRect(int x, int y, int w, int h);
        void clearScreen();

        // Add sprite access methods
        LGFX &getLCD() { return m_lcd; }
        LGFX_Sprite &getSprite() { return m_sprite; }
        LGFX_Sprite &getTextSprite() { return m_text_sprite; }

    private:
        LGFX m_lcd;
        LGFX_Sprite m_sprite;
        LGFX_Sprite m_text_sprite;
        uint8_t m_brightness_index;
    };

} // namespace keyboard