#pragma once

#include "base.h"
#include "players/Player.h"


extern IMAGE img_buff_icon_hurry;
extern IMAGE img_buff_icon_invisible;
extern IMAGE img_buff_icon_recover;
extern IMAGE img_buff_icon_silence;

class StatusBar{
public:
    StatusBar(Player* player) : owner(player) {}
    ~StatusBar() = default;
    void setAvatar(IMAGE* avatar) {
        this->avatar = avatar;
    }
    void setPos(int x,int y){
        pos.x = x;
        pos.y = y;
    }
    void draw(){
        if(!visible)return;
        putImage(pos.x, pos.y, avatar);

        setfillcolor(RGB(5,5,5));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+width+3*2,pos.y+36,8,8);
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+width+3*2+3*2,pos.y+71,8,8);
        setfillcolor(RGB(67,47,47));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+width+3,pos.y+33,4,4);
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+width+3,pos.y+68,4,4);

        float hpBarW=width*max(0,owner->hp)/100.0f;
        float mpBarW=width*min(100,owner->mp)/100.0f;

        setfillcolor(RGB(197,61,67));
        solidroundrect(pos.x+100,pos.y+10,pos.x+100+hpBarW+3,pos.y+33,8,8);
        setfillcolor(RGB(83,131,195));
        solidroundrect(pos.x+100,pos.y+45,pos.x+100+mpBarW+3,pos.y+68,8,8);
        
        int icon_height=img_buff_icon_hurry.getheight();
        int icon_width=img_buff_icon_hurry.getwidth();

        if(owner->is_hurrying)putImage(pos.x + 100, pos.y-icon_height, &img_buff_icon_hurry);
        if(owner->is_invisible)putImage(pos.x + 100+icon_width, pos.y-icon_height, &img_buff_icon_invisible);
        if(owner->is_recovering)putImage(pos.x + 100+icon_width*2, pos.y-icon_height, &img_buff_icon_recover);
        if(owner->is_silenced)putImage(pos.x + 100+icon_width*3, pos.y-icon_height, &img_buff_icon_silence);
    }
    void setVisible(bool val){
        visible=val;
    }
private:
    IMAGE* avatar = nullptr;
    POINT pos={0,0};
    const int width=275;
    Player* owner = nullptr;
    bool visible=true;
};