#pragma once
#include "Timer.h"
#include "Vector2.h"
class Camera
{
public:
    Camera(){
        timer_shake.setOneShot(true);
        timer_shake.set_callback([&]{
            is_shaking = false;
            reset();
        });
    }
    ~Camera()=default;

    const Vector2& getPosition() const {
        return position;
    }
    void reset(){
        position = {0, 0};
    }

    void update(int delta){
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
private:
    Vector2 position;
    float shaking_strength = 0.0f;
    bool is_shaking = false;
    Timer timer_shake;
};