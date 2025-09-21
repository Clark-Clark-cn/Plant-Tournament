#pragma once

#include "baseItem/base.h"
#include "players/Player.h"


extern IMAGE img_buff_icon_hurry;
extern IMAGE img_buff_icon_invisible;
extern IMAGE img_buff_icon_recover;
extern IMAGE img_buff_icon_silence;

extern Camera camera;

class StatusBar{
public:
    StatusBar(Player* player) : owner(player) {}
    ~StatusBar() = default;
    void setAvatar(const IMAGE& avatar) {
        this->avatar = &avatar;
    }
    void setPos(int x,int y){
        pos.x = x;
        pos.y = y;
    }
    void draw(){
        if(!visible)return;
        camera.draw(pos, avatar);

        camera.setColor({5,5,5});
        camera.rect({(int)pos.x+100,(int)pos.y+10,(int)width+3*2,(int)24},true);
        camera.rect({(int)pos.x+100,(int)pos.y+45,(int)width+3*2,(int)24},true);
        camera.setColor({67,47,47});
        camera.rect({(int)pos.x+100,(int)pos.y+10,(int)width+3,(int)23},true);
        camera.rect({(int)pos.x+100,(int)pos.y+45,(int)width+3,(int)23},true);

        float hpBarW=width*std::max(0.0f,owner->hp)/owner->max_hp;
        float mpBarW=width*std::min(100.0f,owner->mp)/100.0f;

        camera.setColor({197,61,67});
        camera.rect({(int)pos.x+100,(int)pos.y+10,(int)hpBarW,(int)23},true);
        camera.setColor({83,131,195});
        camera.rect({(int)pos.x+100,(int)pos.y+45,(int)mpBarW,(int)23},true);
        
        int icon_height=img_buff_icon_hurry.getHeight();
        int icon_width=img_buff_icon_hurry.getWidth();

        if(owner->is_hurrying)camera.draw(pos.x + 100, pos.y-icon_height, &img_buff_icon_hurry);
        if(owner->is_invisible)camera.draw(pos.x + 100+icon_width, pos.y-icon_height, &img_buff_icon_invisible);
        if(owner->is_recovering)camera.draw(pos.x + 100+icon_width*2, pos.y-icon_height, &img_buff_icon_recover);
        if(owner->is_silenced)camera.draw(pos.x + 100+icon_width*3, pos.y-icon_height, &img_buff_icon_silence);
    }
    void setVisible(bool val){
        visible=val;
    }
private:
    const IMAGE* avatar = nullptr;
    Vector2 pos={0,0};
    const int width=275;
    Player* owner = nullptr;
    bool visible=true;
};