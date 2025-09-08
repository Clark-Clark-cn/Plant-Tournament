#pragma once
#include "Bullet.h"

extern Atlas atlas_nut_explode;

class NutExplode:public Bullet
{
public:
    NutExplode()
    {
        once=false;
        size = {288,288};
        damage = 50;
        animation_explode.setAtlas(&atlas_nut_explode);
        animation_explode.setInterval(100);
        animation_explode.setLoop(false);
        animation_explode.setCallback([&](){
            isValid=false;
            canRemove=true;
        });
    }
    ~NutExplode()=default;
    void update(int delta)override{
        Bullet::update(delta);
        animation_explode.update(delta);
        if(checkIfExceedsScreen())canRemove=true;
    }
    void draw(const Camera& camera)const override{
        if(isValid)animation_explode.draw(camera, (int)position.x, (int)position.y);
        Bullet::draw(camera);
    }
private:
    Animation animation_explode;
};
