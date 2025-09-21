#pragma once
#include "Timer.h"
#include "Vector2.h"
#include "base.h"
#include <SDL.h>
class Camera
{
public:
    Camera() : renderer(nullptr) {
        timer_shake.setOneShot(true);
        timer_shake.set_callback([&]{
            is_shaking = false;
            reset();
        });
    }
    ~Camera()=default;

    void setRenderer(SDL_Renderer* ren) {
        renderer = ren;
    }
    const Vector2& getPosition() const {
        return position;
    }
    void reset(){
        position = {0, 0};
    }

    void update(float delta){
        timer_shake.update(delta);
        if(is_shaking){
            position.x = (-50+rand()%100)/50.0f*shaking_strength;
            position.y = (-50+rand()%100)/50.0f*shaking_strength;
        }
    }
    void shake(float strength,int duration){
        shaking_strength = strength;
        is_shaking = true;
        timer_shake.setWaitTime(duration);
        timer_shake.restart();
    }
    void draw(SDL_Texture* texture, const SDL_Rect* src, const SDL_FRect* dst,
     const SDL_FPoint* center=nullptr,double angle=0,SDL_RendererFlip flip_mode=SDL_FLIP_NONE)const{
        SDL_FRect dst_win = *dst;
        dst_win.x -= position.x;
        dst_win.y -= position.y;
        
        SDL_RenderCopyExF(renderer,texture,src,&dst_win,angle,center,flip_mode);
    }
    void draw(const IMAGE* img, const SDL_FRect* dst=nullptr,SDL_RendererFlip flip_mode=SDL_FLIP_NONE,
     const SDL_FPoint* center=nullptr,double angle=0)const{
        SDL_FRect act_dst;
        if(!dst) act_dst = SDL_FRect{0, 0, float(WINDOW_WIDTH), float(WINDOW_HEIGHT)};
        else act_dst = *dst;
        SDL_Rect src = img->getSize();
        draw(img->getTexture(), &src, &act_dst, center, angle, flip_mode);
    }
    void draw(int x,int y,const IMAGE* img, SDL_RendererFlip flip_mode=SDL_FLIP_NONE)const{
        SDL_FRect dst = { float(x), float(y), float(img->getWidth()), float(img->getHeight()) };
        draw(img, &dst, flip_mode);
    }
    void draw(const Vector2& point,const IMAGE* img, SDL_RendererFlip flip_mode=SDL_FLIP_NONE)const{
        draw(point.x,point.y,img, flip_mode);
    }
    void line(const Vector2& start, const Vector2& end)const{
        SDL_RenderDrawLine(renderer, int(start.x - position.x), int(start.y - position.y),
         int(end.x - position.x), int(end.y - position.y));
    }
    void rect(const SDL_Rect& rect, bool filled=false)const{
        SDL_Rect draw_rect = { rect.x - int(position.x), rect.y - int(position.y), rect.w, rect.h };
        if(filled)SDL_RenderFillRect(renderer, &draw_rect);
        else SDL_RenderDrawRect(renderer, &draw_rect);
    }
    void circle(const Vector2& center, int radius, bool filled=false)const{
        if(filled) drawFilledCircle(center, radius);
        else drawCircleOutline(center, radius);
    }
    void setColor(const Color& color)const{
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }
private:
    Vector2 position;
    float shaking_strength = 0.0f;
    bool is_shaking = false;
    Timer timer_shake;
    SDL_Renderer* renderer = nullptr;

    void drawCircleOutline(const Vector2& center, int radius)const{
        int y = radius;
        int d = 1-radius;
        drawCirclePoints(center,0,y);
        for(int x=0;x<y;x++){
            if(d<0)d+=2*x+3;
            else {d+=2*(x-y)+5;y--;}
            drawCirclePoints(center,x,y);
        }
    }
    void drawCirclePoints(const Vector2& center, int x, int y)const{
        SDL_Point points[8]={
            {(int)center.x+x,(int)center.y+y},
            {(int)center.x-x,(int)center.y+y},
            {(int)center.x+x,(int)center.y-y},
            {(int)center.x-x,(int)center.y-y},
            {(int)center.x+y,(int)center.y+x},
            {(int)center.x-y,(int)center.y+x},
            {(int)center.x+y,(int)center.y-x},
            {(int)center.x-y,(int)center.y-x}};
        SDL_RenderDrawPoints(renderer, points, 8);
    }
    void drawFilledCircle(const Vector2& center, int radius)const{
        int y=radius;
        int d=1-radius;

        SDL_RenderDrawLine(renderer,center.x-radius,center.y,center.x+radius,center.y);
        for(int x=0;x<y;x++){
            if(d<0)d+=2*x+3;
            else {
                d+=2*(x-y)+5;
                y--;
            }
            SDL_RenderDrawLine(renderer, center.x-x, center.y+y, center.x+x, center.y+y);
            SDL_RenderDrawLine(renderer, center.x-x, center.y-y, center.x+x, center.y-y);
            SDL_RenderDrawLine(renderer, center.x-y,center.y+x,center.x+y,center.y+x);
            SDL_RenderDrawLine(renderer,center.x-y,center.y-x,center.x+y,center.y-x);
        }
    }
};