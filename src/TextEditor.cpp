#include "TextEditor.hpp"
#include "Display.hpp"

namespace keyboard
{

    TextEditor::TextEditor()
        : m_mode(Mode::FileName), m_caps_lock(false), m_current_line(0), m_max_line(0), m_file_name("untitled.txt"), m_draw_line(true)
    {
        reset();
    }

    void TextEditor::reset()
    {
        m_current_line = 0;
        m_max_line = 0;
        m_file_name = "untitled.txt";
        m_text_lines.fill("");
        m_spaces.fill("|");
        m_draw_line = true;
    }

    void TextEditor::handleKeyPress(uint8_t ascii, uint8_t keycode, uint8_t modifier)
    {
        if (m_mode == Mode::FileName)
        {
            handleFileNameInput(ascii, keycode);
        }
        else
        {
            handleTextInput(ascii, keycode);
        }
    }

    void TextEditor::handleFileNameInput(uint8_t ascii, uint8_t keycode)
    {
        if ('!' <= ascii && ascii <= '~' && m_file_name.length() < 18)
        {
            char c = m_caps_lock ? ascii : (ascii - 32);
            m_file_name += c;
        }
        else if (keycode == 42 && !m_file_name.empty())
        { // Backspace
            m_file_name.pop_back();
        }
    }

    void TextEditor::handleTextInput(uint8_t ascii, uint8_t keycode)
    {
        if ('!' <= ascii && ascii <= '~')
        {
            char c = m_caps_lock ? ascii : (ascii - 32);
            m_text_lines[m_current_line] += c;
            m_spaces[m_current_line] = " " + m_spaces[m_current_line];
        }
        else if (keycode == 44)
        { // Space
            m_text_lines[m_current_line] += " ";
            m_spaces[m_current_line] = " " + m_spaces[m_current_line];
        }
        else if (keycode == 43)
        { // Tab
            m_text_lines[m_current_line] += "   ";
            m_spaces[m_current_line] = "   " + m_spaces[m_current_line];
        }
        else if (keycode == 42 && !m_text_lines[m_current_line].empty())
        { // Backspace
            m_text_lines[m_current_line].pop_back();
            m_spaces[m_current_line] = m_spaces[m_current_line].substr(1);
        }
        else if (keycode == 76)
        { // Delete
            m_text_lines[m_current_line].clear();
            m_spaces[m_current_line] = "|";
        }

        m_max_line = std::max(m_max_line, m_current_line);
    }

    void TextEditor::toggleMode()
    {
        m_mode = (m_mode == Mode::FileName) ? Mode::TextEdit : Mode::FileName;
    }

    void TextEditor::toggleCapsLock()
    {
        m_caps_lock = !m_caps_lock;
    }

    void TextEditor::draw(Display &display)
    {
        // Draw mode indicators
        display.fillRect(30, 65, 8, 12, m_mode == Mode::FileName ? TFT_RED : 0x0016);
        display.fillRect(30, 12, 8, 12, m_mode == Mode::TextEdit ? TFT_RED : 0x0016);

        // Draw current line exactly as in original code
        auto &sprite = display.getSprite();
        if (m_draw_line)
            sprite.fillSprite(1);
        else
            sprite.fillSprite(0);

        sprite.setTextSize(2);
        sprite.setTextColor(4);
        sprite.drawString(("#" + std::to_string(m_current_line + 1) + ".").c_str(), 4, 10);
        sprite.setTextColor(7);
        if (m_draw_line)
            sprite.drawString(m_spaces[m_current_line].c_str(), 64, 10);
        sprite.setTextColor(2);
        sprite.drawString(m_text_lines[m_current_line].c_str(), 64, 10);
        sprite.pushSprite(65, 70 + (m_current_line * 30));
    }

    void TextEditor::drawFileName(Display &display)
    {
        // Draw filename exactly as in original code
        auto &sprite = display.getTextSprite();
        sprite.fillSprite(0);
        sprite.setTextColor(1);
        sprite.setTextSize(3);
        sprite.drawString(m_file_name.c_str(), 8, 8);
        sprite.pushSprite(110, 6);
    }

    void TextEditor::updateLine()
    {
        m_draw_line = !m_draw_line;
    }

    void TextEditor::loadContent(const std::vector<std::string> &content)
    {
        reset();
        size_t lines_to_copy = std::min(content.size(), static_cast<size_t>(config::MAX_LINES));
        for (size_t i = 0; i < lines_to_copy; i++)
        {
            m_text_lines[i] = content[i];
            m_spaces[i] = std::string(content[i].length(), ' ') + "|";
        }
        m_max_line = lines_to_copy - 1;
        m_current_line = 0;
    }

} // namespace keyboard