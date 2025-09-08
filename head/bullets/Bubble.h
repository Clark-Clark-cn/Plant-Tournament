#pragma once

#include "Bullet.h"
#include "../Camera.h"
#include "../players/Player.h"

extern Atlas atlas_bubble;

class Bubble:public Bullet{
public:
    Bubble(Player* owner):Bullet(){
        this->owner = owner;
        once=false;
        size.x=192;
        size.y=192;
        damage=25;
        animation_bubble.setAtlas(&atlas_bubble);
        animation_bubble.setInterval(75);
        animation_bubble.setLoop(false);
        animation_bubble.setCallback([this](){
            isValid=false;
            canRemove=true;
        });
    }
    ~Bubble()=default;
    void update(int delta)override{
        animation_bubble.update(delta);
        if(isValid){
            Vector2 newPos=owner->getPosition();
            const Vector2& playerSize= owner->getSize();
            newPos+=(playerSize-size)/2;
            position=newPos;
        }
        if(checkIfExceedsScreen())canRemove=true;
    }
    void draw(const Camera& camera)const override{
        if(isValid)animation_bubble.draw(camera, (int)position.x, (int)position.y);
        Bullet::draw(camera);
    }
private:
    Animation animation_bubble;
    Player* owner;
};