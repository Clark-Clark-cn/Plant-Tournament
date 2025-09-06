#pragma once

#include "base.h"

class StatusBar{
public:
    StatusBar() = default;
    ~StatusBar() = default;
    void setAvatar(IMAGE* avatar) {
        this->avatar = avatar;
    }
    void setPos(int x,int y){
        pos.x = x;
        pos.y = y;
    }
    void setHP(int hp) {
        this->hp = hp;
    }
    void setMP(int mp) {
        this->mp = mp;
    }
    void draw(){
        putImage(pos.x, pos.y, avatar);

        setfillcolor(RGB(5,5,5));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+width+3*2,pos.y+36,8,8);
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+width+3*2+3*2,pos.y+71,8,8);
        setfillcolor(RGB(67,47,47));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+width+3,pos.y+33,4,4);
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+width+3,pos.y+68,4,4);

        float hpBarW=width*max(0,hp)/100.0f;
        float mpBarW=width*min(100,mp)/100.0f;

        setfillcolor(RGB(197,61,67));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+hpBarW+3,pos.y+33,8,8);
        setfillcolor(RGB(83,131,195));
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+mpBarW+3,pos.y+68,8,8);
    }
private:
    IMAGE* avatar = nullptr;
    POINT pos={0,0};
    int hp = 100;
    int mp = 0;
    const int width=275;
};