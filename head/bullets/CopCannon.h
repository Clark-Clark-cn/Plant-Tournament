#pragma once

#include "Bullet.h"
#include "../players/Player.h"

extern IMAGE img_cop_cannon;
extern Atlas atlas_nut_explode;

extern Player* player_1;
extern Player* player_2;

extern Audio boom;

class CopCannon:public Bullet
{
public:
    CopCannon(){
        size.x=155,size.y=75;
        damage=80;
        animation_explode.setAtlas(&atlas_nut_explode);
        animation_explode.setInterval(100);
        animation_explode.setLoop(false);
        animation_explode.setCallback([&]{canRemove=true;});
        const IMAGE* img=atlas_nut_explode.getImage(0);
        explode_size={(float)img->getWidth(),(float)img->getHeight()};
    }
    ~CopCannon()=default;
    void collide()override{
        boom.play();
        velocity=Vector2(0,0);
        Vector2 player_position,player_size;
        if(collideTarget==PlayerID::P1){
            player_position=player_1->getPosition();
            player_size=player_1->getSize();
        }
        else{
            player_position=player_2->getPosition();
            player_size=player_2->getSize();
        }
        Vector2 bullet_position=player_position+(player_size-explode_size)/2;
        setPosition(bullet_position);
        Bullet::collide();
    }
    void update(float delta) override {
        position+=velocity*(float)delta;
        if(!isValid)animation_explode.update(delta);
        if(checkIfExceedsScreen())canRemove=true;
    }
    void draw(const Camera& camera) const override{
        if(isValid){
            camera.draw(position,&img_cop_cannon);
        }
        else animation_explode.draw(camera,position.x,position.y);
    }
private:
    Vector2 direction;
    Vector2 explode_size;
    Animation animation_explode;
};