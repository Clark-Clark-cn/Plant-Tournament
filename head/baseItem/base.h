#pragma once

#include <iostream>
#include "Image.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern bool is_debug;

class Color{
public:
    Uint8 r,g,b,a;
    Color() :r(255), g(255), b(255), a(255) {}
    Color(Uint32 color) {
        r = (color >> 24) & 0xFF;
        g = (color >> 16) & 0xFF;
        b = (color >> 8) & 0xFF;
        a = color & 0xFF;
    }
    Color(Uint8 r=255,Uint8 g=255,Uint8 b=255,Uint8 a=255):r(r),g(g),b(b),a(a){}
    
    operator SDL_Color() const { return SDL_Color{r,g,b,a}; }
    operator Uint32() const { return (Uint32(r) << 24) | (Uint32(g) << 16) | (Uint32(b) << 8) | Uint32(a); }
};