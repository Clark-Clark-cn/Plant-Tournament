#pragma once

#include"Bullet.h"
#include "baseItem/Camera.h"

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

extern Camera camera;

extern Audio sun_explode_ex;

class SunBulletEx:public Bullet{
    Animation animation_idle;
    Animation animation_break;
    Vector2 breakRenderOffset;
public:
    SunBulletEx(){
        size.x=288;
        size.y=288;
        damage=40;

        animation_idle.setAtlas(&atlas_sun_ex);
        animation_idle.setInterval(50);
        animation_idle.setLoop(true);

        animation_break.setAtlas(&atlas_sun_ex_explode);
        animation_break.setInterval(50);
        animation_break.setLoop(false);
        animation_break.setCallback([&]{canRemove=true;});
        const IMAGE* frameIdle = animation_idle.getFrame();
        const IMAGE* frameBreak = animation_break.getFrame();
        breakRenderOffset.x = (frameIdle->getWidth() - frameBreak->getWidth()) / 2.0f;
        breakRenderOffset.y = (frameIdle->getHeight() - frameBreak->getHeight()) / 2.0f;
    }
    ~SunBulletEx()=default;

    virtual void collide() override{
        sun_explode_ex.play();
        Bullet::collide();
        camera.shake(20,350);
    }
    bool checkCollision(const Vector2& position,const Vector2& size) override{
        if(!isValid)return false;
        return (std::max(this->position.x + this->size.x, position.x + size.x) - std::min(this->position.x, position.x))
               <= this->size.x + size.x &&
               (std::max(this->position.y + this->size.y, position.y + size.y) - std::min(this->position.y, position.y))
               <= this->size.y + size.y;
    }
    void update(float delta) override {
        if(isValid){
            position+=velocity*(float)delta;
        }
        if(!isValid)animation_break.update(delta);
        else animation_idle.update(delta);
        if(checkIfExceedsScreen())canRemove=true;
    }

    void draw(const Camera& camera) const override {
        if(isValid)animation_idle.draw(camera,position.x,position.y);
        else {
            animation_break.draw(camera,position.x+breakRenderOffset.x,position.y+breakRenderOffset.y);
        }
        Bullet::draw(camera);
    }

};
