#pragma once

#include <NeoPixelBus.h>
#include "KeyboardConfig.hpp"

namespace keyboard
{

    class NeoPixelManager
    {
    public:
        NeoPixelManager();
        ~NeoPixelManager() = default;

        bool init();
        void setBrightness(uint8_t brightness);
        void setPixel(uint16_t index, uint32_t color);
        void setAllPixels(uint32_t color);
        void clear();
        void show();

    private:
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> m_strip;
    };

} // namespace keyboard