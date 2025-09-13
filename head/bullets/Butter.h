#pragma once

#include "Bullet.h"

extern IMAGE img_butter;

extern Player* player_1;
extern Player* player_2;

class Butter:public Bullet
{
    const float speed=0.5f;
    const float speed_diagonal=speed/std::sqrt(2.0f);
public:
    enum class Direction{
        left,
        right,
        up,
        down,
        up_left,
        up_right,
        down_left,
        down_right
    };
    Butter(Direction dir=Direction::right):Bullet()
    {
        switch(dir){
            case Direction::left:
                velocity=Vector2(-speed,0);
                break;
            case Direction::right:
                velocity=Vector2(speed,0);
                break;
            case Direction::up:
                velocity=Vector2(0,-speed);
                break;
            case Direction::down:
                velocity=Vector2(0,speed);
                break;
            case Direction::up_left:
                velocity=Vector2(-speed_diagonal,-speed_diagonal);
                break;
            case Direction::up_right:
                velocity=Vector2(speed_diagonal,-speed_diagonal);
                break;
            case Direction::down_left:
                velocity=Vector2(-speed_diagonal,speed_diagonal);
                break;
            case Direction::down_right:
                velocity=Vector2(speed_diagonal,speed_diagonal);
                break;
        }
        size=Vector2(50,50);
    }
    ~Butter()=default;
    void update(int delta) override {
        position+=velocity*(float)delta;
        if(!isValid)canRemove=true;
        if(checkIfExceedsScreen())canRemove=true;
    }
    void draw(const Camera& camera) const override{
        putImage(camera,position.x,position.y,&img_butter);
    }
    void collide() override{
        mciSendString(L"play butter from 0", NULL, 0, NULL);
        if(collideTarget==PlayerID::P2)player_2->set_buttered(true);
        else player_1->set_buttered(true);
        isValid=false;
        canRemove=true;
    }
};