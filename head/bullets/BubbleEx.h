#pragma once

#include "Bullet.h"
#include "../Camera.h"
#include "../players/Player.h"

extern Atlas atlas_bubble_ex;

class BubbleEx:public Bullet{
public:
    BubbleEx(Player* owner):Bullet(){
        this->owner = owner;
        once=false;
        size.x=384;
        size.y=384;
        damage=30;
        animation_bubble_ex.setAtlas(&atlas_bubble_ex);
        animation_bubble_ex.setInterval(75);
        animation_bubble_ex.setLoop(true);
        life_timer.setWaitTime(life_duration);
        life_timer.setOneShot(true);
        life_timer.set_callback([this](){
            isValid=false;
            canRemove=true;
        });
        life_timer.restart();
    }
    ~BubbleEx()=default;
    void update(int delta)override{
        animation_bubble_ex.update(delta);
        life_timer.update(delta);
        if(isValid){
            Vector2 newPos=owner->getPosition();
            const Vector2& playerSize= owner->getSize();
            newPos+=(playerSize-size)/2;
            position=newPos;
        }
        if(checkIfExceedsScreen())canRemove=true;
    }
    void draw(const Camera& camera)const override{
        if(isValid)animation_bubble_ex.draw(camera, (int)position.x, (int)position.y);
        Bullet::draw(camera);
    }
private:
    Animation animation_bubble_ex;
    Timer life_timer;
    const int life_duration=3000; // milliseconds
    Player* owner;
};