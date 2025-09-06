#pragma once

#include <graphics.h>
#include <functional>

#include "Atlas.h"
#include "Camera.h"
#include "base.h"

class Animation {
    Atlas* atlas=nullptr;
    int interval_ms=0;
    int frameCount=0;
    int timer=0;
    int isLoop=0;
    std::function<void()> onFinished;
public:
    Animation()=default;
    ~Animation()=default;

    void reset(){
        timer = 0;
        frameCount = 0;
    }
    void setAtlas(Atlas* newAtlas) {
        reset();
        atlas = newAtlas;
    }
    void setLoop(bool loop) {
        isLoop = loop;
    }
    void setInterval(int interval) {
        interval_ms = interval;
    }
    int getIndexFrame() const {
        return frameCount;
    }
    IMAGE* getFrame()const {
        return atlas->getImage(frameCount);
    }
    bool checkFinished() const {
        if(isLoop)return false;
        return frameCount >= atlas->size()-1;
    }
    void setCallback(std::function<void()> callback) {
        onFinished = callback;
    }
    void update(int delta) {
        timer += delta;
        if(timer>=interval_ms){
            timer=0;
            frameCount++;
            if(frameCount>=atlas->size()){
                frameCount=isLoop?0:atlas->size()-1;
                if(!isLoop && onFinished) onFinished();
            }
        }
    }
    void draw(const Camera& camera,int x, int y)const{
        putImage(camera,x,y,getFrame());
    }
};