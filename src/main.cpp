#include "KeyboardManager.hpp"
#include "esp_task_wdt.h"
#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "Main";

keyboard::KeyboardManager g_keyboard_manager;

void setup()
{
    Serial.begin(115200);
    esp_log_level_set("*", ESP_LOG_INFO); // Set global log level

    ESP_LOGI(TAG, "Starting keyboard application...");
    ESP_LOGI(TAG, "Configuring watchdog...");

    // Configure watchdog with a 10 second timeout (increased from 5)
    ESP_ERROR_CHECK(esp_task_wdt_init(10, true));
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    ESP_LOGI(TAG, "Watchdog configured with 10s timeout");

    // Initialize keyboard manager with retry
    int retry_count = 0;
    const int max_retries = 3;

    while (retry_count < max_retries)
    {
        if (g_keyboard_manager.init())
        {
            ESP_LOGI(TAG, "Keyboard manager initialized successfully");
            break;
        }

        ESP_LOGW(TAG, "Failed to initialize keyboard manager (attempt %d/%d)",
                 retry_count + 1, max_retries);

        retry_count++;
        if (retry_count < max_retries)
        {
            ESP_LOGI(TAG, "Retrying in 1 second...");
            esp_task_wdt_reset();
            delay(1000);
        }
    }

    if (retry_count >= max_retries)
    {
        ESP_LOGE(TAG, "Failed to initialize keyboard manager after %d attempts", max_retries);
        ESP_LOGE(TAG, "System will restart in 5 seconds");

        // Blink rapidly to indicate error
        for (int i = 0; i < 10; i++)
        {
            esp_task_wdt_reset();
            delay(500);
        }

        ESP_LOGE(TAG, "Restarting system...");
        esp_restart();
    }
}

void loop()
{
    static uint32_t last_watchdog_reset = 0;
    uint32_t current_time = millis();

    // Reset watchdog timer every 1 second
    if (current_time - last_watchdog_reset >= 1000)
    {
        esp_task_wdt_reset();
        last_watchdog_reset = current_time;
    }

    // Process keyboard manager tasks
    g_keyboard_manager.process();
}