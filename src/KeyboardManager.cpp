#include "KeyboardManager.hpp"
#include <vector>
#include "esp_log.h"

static const char *TAG = "KeyboardManager";

namespace keyboard
{

    KeyboardManager::KeyboardManager()
    {
    }

    bool KeyboardManager::init()
    {
        ESP_LOGI(TAG, "Initializing keyboard manager...");

        // Initialize display
        ESP_LOGI(TAG, "Initializing display...");
        m_display.init();
        ESP_LOGI(TAG, "Display initialized");

        // Draw initial UI
        ESP_LOGI(TAG, "Drawing initial UI...");
        m_editor.draw(m_display);         // Draw initial editor UI
        m_editor.drawFileName(m_display); // Draw initial filename
        ESP_LOGI(TAG, "Initial UI drawn");

        // Initialize NeoPixels
        ESP_LOGI(TAG, "Initializing NeoPixels...");
        if (!m_pixels.init())
        {
            ESP_LOGE(TAG, "Failed to initialize NeoPixels!");
            return false;
        }
        ESP_LOGI(TAG, "NeoPixels initialized");

        // Initialize file system
        ESP_LOGI(TAG, "Initializing file system...");
        if (!m_file_manager.init())
        {
            ESP_LOGE(TAG, "Failed to initialize file system!");
            return false;
        }
        ESP_LOGI(TAG, "File system initialized");

        // Initialize USB host
        ESP_LOGI(TAG, "Initializing USB host...");
        EspUsbHost::begin();
        ESP_LOGI(TAG, "USB host initialized");

        ESP_LOGI(TAG, "Keyboard manager initialization complete");
        return true;
    }

    void KeyboardManager::process()
    {
        static uint32_t last_process_time = 0;
        uint32_t current_time = millis();

        // Process USB host tasks
        EspUsbHost::task();

        // Log if processing takes too long
        uint32_t process_time = millis() - current_time;
        if (process_time > 100)
        { // Log if processing takes more than 100ms
            ESP_LOGW(TAG, "Long process time: %lu ms", process_time);
        }

        // Give other tasks a chance to run and feed the watchdog
        vTaskDelay(pdMS_TO_TICKS(1));

        last_process_time = current_time;
    }

    void KeyboardManager::reset()
    {
        m_editor.reset();
        m_file_manager.setMenuOpen(false);
        m_display.clearScreen();
    }

    void KeyboardManager::onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier)
    {
        if (keycode >= 58 && keycode <= 69)
        { // F1-F12
            handleFunctionKey(keycode);
            return;
        }

        if (keycode == 57)
        { // Caps Lock
            m_editor.toggleCapsLock();
            m_display.fillRect(175, 575, 20, 15, m_editor.isCapsLock() ? TFT_RED : TFT_SILVER);
            return;
        }

        if (keycode == 60)
        { // F3 - Mode toggle
            m_editor.toggleMode();
            return;
        }

        if (m_file_manager.isMenuOpen())
        {
            handleNavigationKey(keycode);
        }
        else
        {
            handleTextInput(ascii, keycode);
        }
    }

    void KeyboardManager::handleFunctionKey(uint8_t keycode)
    {
        // Move vector outside switch to avoid initialization crossing
        std::vector<std::string> files;
        m_file_manager.listFiles(files);

        switch (keycode)
        {
        case 58: // F1 - Reset
            reset();
            break;

        case 59: // F2 - Open file menu
            m_file_manager.setMenuOpen(true);
            reset();
            m_display.getLCD().setTextSize(2);
            for (size_t i = 0; i < files.size(); i++)
            {
                m_display.drawText(files[i], 90, 80 + (i * 30));
            }
            m_display.fillRect(70, 83 + m_file_manager.getSelectedFileIndex() * 30, 10, 10, TFT_YELLOW);
            break;

        case 61:
        { // F4 - Save file
            std::vector<std::string> content;
            for (int i = 0; i <= m_editor.getCurrentLine(); i++)
            {
                content.push_back(m_editor.getLine(i));
            }
            m_file_manager.saveFile(m_editor.getCurrentFileName(), content);
            break;
        }

        case 62: // F5 - Cycle brightness
            m_display.cycleBrightness();
            break;
        }
    }

    void KeyboardManager::handleNavigationKey(uint8_t keycode)
    {
        // Move vector outside switch to avoid initialization crossing
        std::vector<std::string> files;
        m_file_manager.listFiles(files);

        switch (keycode)
        {
        case 40:
        { // Enter
            reset();
            if (m_file_manager.getSelectedFileIndex() < static_cast<int>(files.size()))
            {
                std::vector<std::string> content;
                if (m_file_manager.openFile(files[m_file_manager.getSelectedFileIndex()], content))
                {
                    m_editor.loadContent(content);
                }
            }
            m_file_manager.setMenuOpen(false);
            break;
        }

        case 81: // Down
            m_file_manager.incrementSelectedFile();
            m_display.fillRect(70, 83 + (m_file_manager.getSelectedFileIndex() - 1) * 30, 10, 10, TFT_BLACK);
            m_display.fillRect(70, 83 + m_file_manager.getSelectedFileIndex() * 30, 10, 10, TFT_YELLOW);
            break;

        case 82: // Up
            m_file_manager.decrementSelectedFile();
            m_display.fillRect(70, 83 + (m_file_manager.getSelectedFileIndex() + 1) * 30, 10, 10, TFT_BLACK);
            m_display.fillRect(70, 83 + m_file_manager.getSelectedFileIndex() * 30, 10, 10, TFT_YELLOW);
            break;
        }
    }

    void KeyboardManager::handleTextInput(uint8_t ascii, uint8_t keycode)
    {
        m_editor.handleKeyPress(ascii, keycode, 0);
        m_editor.draw(m_display);
    }

} // namespace keyboard