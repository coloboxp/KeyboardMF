#pragma once

#include <string>
#include <array>
#include <vector>
#include "KeyboardConfig.hpp"

namespace keyboard
{

    class Display;

    class TextEditor
    {
    public:
        enum class Mode
        {
            FileName,
            TextEdit
        };

        TextEditor();
        ~TextEditor() = default;

        void reset();
        void handleKeyPress(uint8_t ascii, uint8_t keycode, uint8_t modifier);
        bool saveFile();
        bool openFile(const std::string &filename);
        void draw(Display &display);
        void drawFileName(Display &display);
        void updateLine();
        void loadContent(const std::vector<std::string> &content);

        bool isFileMode() const { return m_mode == Mode::FileName; }
        void toggleMode();
        bool isCapsLock() const { return m_caps_lock; }
        void toggleCapsLock();

        const std::string &getCurrentFileName() const { return m_file_name; }
        int getCurrentLine() const { return m_current_line; }
        const std::string &getLine(int index) const { return m_text_lines[index]; }

    private:
        void handleFileNameInput(uint8_t ascii, uint8_t keycode);
        void handleTextInput(uint8_t ascii, uint8_t keycode);

        Mode m_mode;
        bool m_caps_lock;
        int m_current_line;
        int m_max_line;
        std::string m_file_name;
        std::array<std::string, config::MAX_LINES> m_text_lines;
        std::array<std::string, config::MAX_LINES> m_spaces;
        bool m_draw_line;
    };

} // namespace keyboard