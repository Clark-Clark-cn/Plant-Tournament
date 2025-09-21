#pragma once

#include "baseItem/Atlas.h"
#include "baseItem/Vector2.h"

class Particle{
public:
    Particle()=default;
    Particle(const Vector2& position, Atlas* atlas, int lifespan)
        : position(position), atlas(atlas), lifespan(lifespan) {}
    ~Particle()=default;

    void setAtlas(Atlas* atlas) {
        this->atlas = atlas;
    }
    void setPosition(const Vector2& position) {
        this->position = position;
    }
    void setLifespan(int lifespan) {
        this->lifespan = lifespan;
    }
    bool checkValid() const {
        return valid;
    }
    void update(float delta){
        timer+=delta;
        if(timer>=lifespan){
            timer=0;
            frameCount++;
            if(frameCount>=atlas->size()){
                valid=false;
            }
        }
    }
    void draw(const Camera& camera) const {
        camera.draw(position,atlas->getImage(frameCount));
    }
private:
    Vector2 position={0, 0};
    Atlas* atlas=nullptr;
    int lifespan=0;
    bool valid = true;
    int frameCount=0;
    int timer=0;
};