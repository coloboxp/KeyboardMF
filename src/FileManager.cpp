#include "FileManager.hpp"
#include "esp_log.h"
#include <LittleFS.h>

static const char *TAG = "FileManager";

namespace keyboard
{
    FileManager::FileManager() : m_selected_file_index(0), m_menu_open(false)
    {
        ESP_LOGI(TAG, "FileManager instance created");
    }

    bool FileManager::init()
    {
        ESP_LOGI(TAG, "Initializing file system...");

        try
        {
            if (!LittleFS.begin(true))
            {
                ESP_LOGE(TAG, "Failed to mount LittleFS");
                return false;
            }

            ESP_LOGI(TAG, "LittleFS mounted successfully");
            ESP_LOGD(TAG, "Total space: %lu bytes", LittleFS.totalBytes());
            ESP_LOGD(TAG, "Used space: %lu bytes", LittleFS.usedBytes());

            return true;
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "File system initialization failed: %s", e.what());
            return false;
        }
    }

    void FileManager::listFiles(std::vector<std::string> &files)
    {
        ESP_LOGD(TAG, "Listing files in root directory");
        files.clear();

        try
        {
            File root = LittleFS.open("/");
            if (!root)
            {
                ESP_LOGE(TAG, "Failed to open root directory");
                return;
            }

            File file = root.openNextFile();
            while (file)
            {
                if (!file.isDirectory())
                {
                    std::string filename = file.name();
                    ESP_LOGV(TAG, "Found file: %s", filename.c_str());
                    files.push_back(filename);
                }
                file = root.openNextFile();
            }

            ESP_LOGI(TAG, "Found %d files", files.size());
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "Error listing files: %s", e.what());
        }
    }

    bool FileManager::openFile(const std::string &filename, std::vector<std::string> &content)
    {
        ESP_LOGI(TAG, "Opening file: %s", filename.c_str());
        content.clear();

        try
        {
            File file = LittleFS.open(("/" + filename).c_str(), "r");
            if (!file)
            {
                ESP_LOGE(TAG, "Failed to open file: %s", filename.c_str());
                return false;
            }

            String line;
            while (file.available())
            {
                line = file.readStringUntil('\n');
                content.push_back(line.c_str());
                ESP_LOGV(TAG, "Read line: %s", line.c_str());
            }

            ESP_LOGI(TAG, "Successfully read %d lines from file", content.size());
            file.close();
            return true;
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "Error reading file: %s", e.what());
            return false;
        }
    }

    bool FileManager::saveFile(const std::string &filename, const std::vector<std::string> &content)
    {
        ESP_LOGI(TAG, "Saving file: %s", filename.c_str());

        try
        {
            File file = LittleFS.open(("/" + filename).c_str(), "w");
            if (!file)
            {
                ESP_LOGE(TAG, "Failed to open file for writing: %s", filename.c_str());
                return false;
            }

            for (const auto &line : content)
            {
                if (file.println(line.c_str()) == 0)
                {
                    ESP_LOGE(TAG, "Failed to write line to file");
                    file.close();
                    return false;
                }
                ESP_LOGV(TAG, "Wrote line: %s", line.c_str());
            }

            file.close();
            ESP_LOGI(TAG, "Successfully wrote %d lines to file", content.size());
            return true;
        }
        catch (const std::exception &e)
        {
            ESP_LOGE(TAG, "Error saving file: %s", e.what());
            return false;
        }
    }

    void FileManager::incrementSelectedFile()
    {
        m_selected_file_index++;
        ESP_LOGV(TAG, "Incremented selected file index to: %d", m_selected_file_index);
    }

    void FileManager::decrementSelectedFile()
    {
        if (m_selected_file_index > 0)
        {
            m_selected_file_index--;
            ESP_LOGV(TAG, "Decremented selected file index to: %d", m_selected_file_index);
        }
    }

} // namespace keyboard