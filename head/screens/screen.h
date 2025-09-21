#pragma once

#include "baseItem/Camera.h"
class Screen
{
public:
    Screen()=default;
    ~Screen()=default;
    virtual void enter(){}
    virtual void update(float delta){}
    virtual void exit(){}
    virtual void draw(const Camera& camera) {}
    virtual void input(const SDL_Event& msg) {}
};
