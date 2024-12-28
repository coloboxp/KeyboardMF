#pragma once

#include <string>
#include <vector>
#include <SD_MMC.h>
#include "KeyboardConfig.hpp"

namespace keyboard
{

    class FileManager
    {
    public:
        FileManager();
        ~FileManager() = default;

        bool init();
        bool saveFile(const std::string &filename, const std::vector<std::string> &content);
        bool openFile(const std::string &filename, std::vector<std::string> &content);
        void listFiles(std::vector<std::string> &files);

        bool isMenuOpen() const { return m_menu_open; }
        void setMenuOpen(bool open) { m_menu_open = open; }

        int getSelectedFileIndex() const { return m_selected_file_index; }
        void setSelectedFileIndex(int index);
        void incrementSelectedFile();
        void decrementSelectedFile();

    private:
        bool m_menu_open;
        int m_selected_file_index;
        std::vector<std::string> m_file_list;

        void updateFileList();
    };

} // namespace keyboard