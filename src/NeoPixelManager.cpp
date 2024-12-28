#include "NeoPixelManager.hpp"
#include "esp_log.h"

static const char *TAG = "NeoPixel";

namespace keyboard
{

    NeoPixelManager::NeoPixelManager()
        : m_strip(config::NEOPIXEL_COUNT, config::NEOPIXEL_PIN)
    {
        ESP_LOGI(TAG, "NeoPixelManager created with %d LEDs on pin %d",
                 config::NEOPIXEL_COUNT, config::NEOPIXEL_PIN);
    }

    bool NeoPixelManager::init()
    {
        ESP_LOGI(TAG, "Initializing NeoPixels...");

        try
        {
            ESP_LOGD(TAG, "Beginning NeoPixel initialization");
            m_strip.Begin();

            ESP_LOGD(TAG, "Setting all pixels to 'off'");
            m_strip.Show();

            ESP_LOGI(TAG, "NeoPixel initialization complete");
            return true;
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "NeoPixel initialization failed: %s", e.what());
            return false;
        }
    }

    void NeoPixelManager::setBrightness(uint8_t brightness)
    {
        ESP_LOGD(TAG, "Setting brightness to %d (Note: Not directly supported by NeoPixelBus)", brightness);
        // NeoPixelBus doesn't have direct brightness control
        // We would need to scale all colors by the brightness value
        // This is left as a future enhancement
    }

    void NeoPixelManager::setPixel(uint16_t index, uint32_t color)
    {
        if (index >= config::NEOPIXEL_COUNT)
        {
            ESP_LOGW(TAG, "Pixel index %d out of range (max: %d)", index, config::NEOPIXEL_COUNT - 1);
            return;
        }

        ESP_LOGV(TAG, "Setting pixel %d to color 0x%06X", index, color);
        RgbColor rgb_color((color >> 16) & 0xFF, // Red
                           (color >> 8) & 0xFF,  // Green
                           color & 0xFF);        // Blue
        m_strip.SetPixelColor(index, rgb_color);
    }

    void NeoPixelManager::setAllPixels(uint32_t color)
    {
        ESP_LOGD(TAG, "Setting all pixels to color 0x%06X", color);
        RgbColor rgb_color((color >> 16) & 0xFF, // Red
                           (color >> 8) & 0xFF,  // Green
                           color & 0xFF);        // Blue

        for (uint16_t i = 0; i < config::NEOPIXEL_COUNT; i++)
        {
            m_strip.SetPixelColor(i, rgb_color);
        }
    }

    void NeoPixelManager::clear()
    {
        ESP_LOGD(TAG, "Clearing all pixels");
        m_strip.ClearTo(RgbColor(0));
    }

    void NeoPixelManager::show()
    {
        ESP_LOGV(TAG, "Updating LED strip");
        try
        {
            m_strip.Show();
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "Failed to update LED strip: %s", e.what());
        }
    }

} // namespace keyboard