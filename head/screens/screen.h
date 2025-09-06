#pragma once

#include "../Camera.h"
class Screen
{
public:
    Screen()=default;
    ~Screen()=default;
    virtual void enter(){}
    virtual void update(int delta){}
    virtual void exit(){}
    virtual void draw(const Camera& camera) {}
    virtual void input(const ExMessage& msg) {}
};
