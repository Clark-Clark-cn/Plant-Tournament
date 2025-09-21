#pragma once

#include "Vector2.h"
#include <SDL_ttf.h>
#include <string>

class Font
{
   public:
    Font() : font_(nullptr) {}
    ~Font() { close(); }

    bool load(SDL_Renderer* renderer, const std::string& path, int ptsize) {
        close();
        renderer_ = renderer;
        font_ = TTF_OpenFont(path.c_str(), ptsize);
        return font_ != nullptr;
    }

    void close() {
        if (font_) {
            TTF_CloseFont(font_);
            font_ = nullptr;
        }
    }

    TTF_Font* get() const { return font_; }
    void drawText(const std::string& text, SDL_Color color, const Vector2& pos) {
        if (!font_ || !renderer_) return;
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font_, text.c_str(), color);
        if (!surface) return;
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
        if (!texture) return;
        SDL_Rect dstRect = {(int)pos.x, (int)pos.y, surface->w, surface->h};
        SDL_RenderCopy(renderer_, texture, nullptr, &dstRect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
    int textwidth(const std::string& str) const {
        if (!font_ || !renderer_) return 0;
        int w = 0, h = 0;
        TTF_SizeText(font_, str.c_str(), &w, &h);
        return w;
    }
   private:
    TTF_Font* font_;
    SDL_Renderer* renderer_;
};

extern Font font_main;