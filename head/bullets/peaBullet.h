#pragma once

#include "Bullet.h"

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet : public Bullet
{
public:
    PeaBullet(){
        size.x=64,size.y=64;
        damage=10;
        animation_break.setAtlas(&atlas_pea_break);
        animation_break.setInterval(100);
        animation_break.setLoop(false);
        animation_break.setCallback([&]{canRemove=true;});
    }
    ~PeaBullet() = default;
    virtual void collide()override{
        std::wstring command=L"play pea_break_"+std::to_wstring(rand()%3)+L"from 0";
        mciSendString(command.c_str(), nullptr, 0, nullptr);
        Bullet::collide();
    }

    void update(int delta) override {
        position+=velocity*(float)delta;
        if(!isValid)animation_break.update(delta);
        if(checkIfExceedsScreen())canRemove=true;
    }

    void draw(const Camera& camera) const override{
        if(isValid)putImage(camera,position.x,position.y,&img_pea);
        else animation_break.draw(camera,position.x,position.y);
    }
private:
	Vector2 direction;
    Animation animation_break;
};
