#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class IMAGE {
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect dst_rect = { 0,0,0,0 };
    int width = 0;
    int height = 0;
public:
    IMAGE() = default;
    IMAGE(SDL_Renderer* renderer, const std::string& filePath): renderer(renderer) { load(renderer, filePath); }
    ~IMAGE() { if (texture)SDL_DestroyTexture(texture); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    SDL_Texture* getTexture() const { return texture; }
    SDL_Rect getSize() const { return dst_rect; }
    void load(SDL_Renderer* renderer, const std::string& filePath) { 
        this->renderer=renderer;
        texture = IMG_LoadTexture(renderer, filePath.c_str());
        if (texture) {
            SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
            dst_rect = { 0,0,width,height };
        }
    }
    void sketch(IMAGE* dst)const{
        if(!texture||!dst)return;
        const int w=width;
        const int h=height;
        if(!dst->create(renderer,w,h, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET))return;

        SDL_Texture* old_target=SDL_GetRenderTarget(renderer);
        SDL_SetRenderTarget(renderer,dst->texture);

        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderClear(renderer);

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);
        SDL_Rect src_rect={0,0,w,h};
        SDL_RenderCopy(renderer,texture,&src_rect,&src_rect);

        SDL_BlendMode toWhite=SDL_ComposeCustomBlendMode(
            SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ZERO,SDL_BLENDOPERATION_ADD,
            SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD
        );
        SDL_SetRenderDrawBlendMode(renderer, toWhite);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
        SDL_RenderFillRect(renderer,&dst_rect);

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer,old_target);
    }
    bool create(SDL_Renderer* renderer, int width , int height,
    Uint32 format=SDL_PIXELFORMAT_ABGR8888,
    int access= SDL_TEXTUREACCESS_TARGET){
        if(!renderer)renderer=this->renderer;
        if(!renderer)return false;

        if(texture){
            SDL_DestroyTexture(texture);
            texture=nullptr;
        }
        texture=SDL_CreateTexture(renderer,format, access,width,height);
        if(!texture)return false;

        this->renderer=renderer;
        this->width=width;
        this->height=height;
        this->dst_rect = { 0, 0, width, height };
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        return true;
    }

    operator SDL_Texture* () const { return texture; }
    operator bool() const { return texture != nullptr; }
};