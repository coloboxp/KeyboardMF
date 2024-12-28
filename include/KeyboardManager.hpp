#pragma once

#include <EspUsbHost.h>
#include "Display.hpp"
#include "TextEditor.hpp"
#include "FileManager.hpp"
#include "NeoPixelManager.hpp"

namespace keyboard
{

    class KeyboardManager : public EspUsbHost
    {
    public:
        KeyboardManager();
        ~KeyboardManager() = default;

        bool init();
        void process();
        void reset();

    protected:
        void onKeyboardKey(uint8_t ascii, uint8_t keycode, uint8_t modifier) override;

    private:
        void handleFunctionKey(uint8_t keycode);
        void handleNavigationKey(uint8_t keycode);
        void handleTextInput(uint8_t ascii, uint8_t keycode);

        Display m_display;
        TextEditor m_editor;
        FileManager m_file_manager;
        NeoPixelManager m_pixels;
    };

} // namespace keyboard