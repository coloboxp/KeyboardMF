#define LGFX_USE_V1
#include "Display.hpp"
#include "esp_log.h"
#include "esp_task_wdt.h"
#include "esp_heap_caps.h"

static const char *TAG = "Display";

namespace keyboard
{

    Display::Display()
        : m_sprite(&m_lcd), m_text_sprite(&m_lcd), m_brightness_index(config::DEFAULT_BRIGHTNESS_INDEX)
    {
        ESP_LOGI(TAG, "Display instance created");
        // Log initial memory state
        ESP_LOGI(TAG, "Free heap: %u bytes", esp_get_free_heap_size());
        ESP_LOGI(TAG, "Free PSRAM: %u bytes", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    }

    void Display::init()
    {
        ESP_LOGI(TAG, "Initializing display...");

        try
        {
            // Initialize LCD with error checking
            ESP_LOGD(TAG, "Starting LCD initialization");
            esp_task_wdt_reset();

            // Log pre-init memory state
            ESP_LOGD(TAG, "Pre-init - Free heap: %u, Free PSRAM: %u",
                     esp_get_free_heap_size(),
                     heap_caps_get_free_size(MALLOC_CAP_SPIRAM));

            ESP_LOGD(TAG, "Configuring LCD parameters");
            try
            {
                // Match original initialization sequence
                m_lcd.init();
                ESP_LOGD(TAG, "LCD init completed successfully");

                ESP_LOGD(TAG, "Setting rotation to 2 (matching original code)");
                m_lcd.setRotation(2);
                ESP_LOGD(TAG, "Rotation set successfully");

                ESP_LOGD(TAG, "Setting initial brightness to %d", config::BRIGHTNESS_LEVELS[m_brightness_index]);
                m_lcd.setBrightness(config::BRIGHTNESS_LEVELS[m_brightness_index]);
                ESP_LOGD(TAG, "Brightness set successfully");

                // Initialize UI elements
                ESP_LOGD(TAG, "Initializing UI elements");

                // Main background
                m_lcd.fillScreen(0x0016);

                // Text areas
                m_lcd.fillRect(55, 60, 920, 500, TFT_BLACK); // Main text area
                m_lcd.fillRect(55, 570, 920, 26, TFT_BLACK); // Bottom status
                m_lcd.fillRect(55, 6, 403, 40, TFT_BLACK);   // Top filename area

                // Borders
                m_lcd.drawRect(48, 4, 410, 44, TFT_SILVER);   // Filename border
                m_lcd.drawRect(48, 564, 932, 36, TFT_SILVER); // Bottom border
                m_lcd.fillRect(170, 570, 30, 24, TFT_SILVER); // Caps lock indicator

                // Mode indicators
                m_lcd.fillRect(24, 6, 20, 24, TFT_SILVER);
                m_lcd.fillRect(27, 9, 14, 18, 0x0016);
                m_lcd.fillRect(24, 14, 20, 8, 0x0016);

                m_lcd.fillRect(24, 59, 20, 24, TFT_SILVER);
                m_lcd.fillRect(27, 62, 14, 18, 0x0016);
                m_lcd.fillRect(24, 67, 20, 8, 0x0016);
                m_lcd.fillRect(30, 90, 9, 506, 0x8410);
                m_lcd.fillRect(30, 37, 9, 14, 0x8410);

                m_lcd.fillRect(30, 12, 8, 12, TFT_RED); // Initial mode indicator

                // Main frame
                m_lcd.fillRect(48, 53, 2, 510, TFT_SILVER);
                m_lcd.fillRect(48, 53, 932, 2, TFT_SILVER);
                m_lcd.fillRect(48, 563, 932, 2, TFT_SILVER);
                m_lcd.fillRect(978, 53, 2, 510, TFT_SILVER);

                // Static text
                m_lcd.setTextSize(2);
                m_lcd.setTextColor(0x7BCF);
                m_lcd.drawString("CopyRight(C) 2024. Volos Projects", 570, 576);
                m_lcd.drawString("CAPSLOCK:", 60, 576);

                // Function buttons
                m_lcd.fillRect(470, 6, 120, 40, TFT_SILVER);
                m_lcd.setTextSize(3);
                m_lcd.setTextColor(TFT_RED);
                m_lcd.drawString("F1", 480, 16);
                m_lcd.setTextColor(TFT_BLACK);
                m_lcd.drawString("NEW", 520, 16);

                m_lcd.fillRect(600, 6, 130, 40, TFT_SILVER);
                m_lcd.setTextColor(TFT_RED);
                m_lcd.drawString("F2", 610, 16);
                m_lcd.setTextColor(TFT_BLACK);
                m_lcd.drawString("OPEN", 650, 16);

                m_lcd.fillRect(740, 6, 130, 40, TFT_SILVER);
                m_lcd.setTextColor(TFT_RED);
                m_lcd.drawString("F4", 750, 16);
                m_lcd.setTextColor(TFT_BLACK);
                m_lcd.drawString("SAVE", 790, 16);

                m_lcd.fillRect(880, 6, 100, 40, TFT_SILVER);
                m_lcd.setTextColor(TFT_RED);
                m_lcd.drawString("F5", 890, 16);
                m_lcd.setTextColor(TFT_BLACK);
                m_lcd.drawString("*", 940, 16);

                // Mode switch indicator
                m_lcd.setTextSize(3);
                m_lcd.setTextColor(TFT_YELLOW);
                m_lcd.drawString("F3", 70, 14);

                ESP_LOGD(TAG, "UI elements initialized");

                // Initialize sprites exactly as in original code
                ESP_LOGD(TAG, "Initializing sprites");
                m_sprite.setColorDepth(4);
                if (!m_sprite.createSprite(900, 30))
                {
                    ESP_LOGE(TAG, "Failed to create line sprite");
                    throw std::runtime_error("Sprite creation failed");
                }
                m_sprite.setPaletteColor(2, 0x00FF00U);

                m_text_sprite.setColorDepth(1);
                if (!m_text_sprite.createSprite(340, 38))
                {
                    ESP_LOGE(TAG, "Failed to create text sprite");
                    throw std::runtime_error("Text sprite creation failed");
                }

                ESP_LOGD(TAG, "Sprites initialized");
                ESP_LOGI(TAG, "Display initialization complete");
            }
            catch (const std::exception &e)
            {
                ESP_LOGE(TAG, "LCD init sequence failed with exception: %s", e.what());
                throw;
            }
            catch (...)
            {
                ESP_LOGE(TAG, "LCD init sequence failed with unknown exception");
                throw;
            }
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "Display initialization failed: %s", e.what());
            throw;
        }
        catch (...)
        {
            ESP_LOGE(TAG, "Display initialization failed with unknown error");
            throw;
        }
    }

    void Display::setBrightness(uint8_t level)
    {
        ESP_LOGD(TAG, "Setting brightness to %d", level);
        m_lcd.setBrightness(level);
    }

    void Display::cycleBrightness()
    {
        m_brightness_index = (m_brightness_index + 1) % (sizeof(config::BRIGHTNESS_LEVELS) / sizeof(config::BRIGHTNESS_LEVELS[0]));
        uint8_t new_brightness = config::BRIGHTNESS_LEVELS[m_brightness_index];
        ESP_LOGI(TAG, "Cycling brightness to index %d (level: %d)", m_brightness_index, new_brightness);
        setBrightness(new_brightness);
    }

    void Display::drawText(const std::string &text, int x, int y, int size, uint16_t color)
    {
        ESP_LOGV(TAG, "Drawing text '%s' at (%d,%d) size %d color 0x%04X", text.c_str(), x, y, size, color);
        m_lcd.setTextSize(size);
        m_lcd.setTextColor(color);
        m_lcd.drawString(text.c_str(), x, y);
    }

    void Display::drawLine(int x1, int y1, int x2, int y2, uint16_t color)
    {
        ESP_LOGV(TAG, "Drawing line from (%d,%d) to (%d,%d) color 0x%04X", x1, y1, x2, y2, color);
        m_lcd.drawLine(x1, y1, x2, y2, color);
    }

    void Display::fillRect(int x, int y, int w, int h, uint16_t color)
    {
        ESP_LOGV(TAG, "Filling rect at (%d,%d) size %dx%d color 0x%04X", x, y, w, h, color);
        m_lcd.fillRect(x, y, w, h, color);
    }

    void Display::clearRect(int x, int y, int w, int h)
    {
        ESP_LOGV(TAG, "Clearing rect at (%d,%d) size %dx%d", x, y, w, h);
        m_lcd.fillRect(x, y, w, h, TFT_BLACK);
    }

    void Display::clearScreen()
    {
        ESP_LOGD(TAG, "Clearing screen");
        m_lcd.fillScreen(TFT_BLACK);
    }

} // namespace keyboard