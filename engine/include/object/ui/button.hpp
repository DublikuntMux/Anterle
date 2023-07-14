#pragma once

#include <stdint.h>

class Button {
public:
    typedef void (*ClickCallback)();

    Button(uint64_t top, uint64_t left, uint64_t width, uint64_t height, ClickCallback onClick);
    ~Button();

    void CheckClick(uint64_t x, uint64_t y);
    
private:
    uint64_t m_width;
    uint64_t m_height;
    uint64_t m_top;
    uint64_t m_left;
    ClickCallback m_onClick;
};
