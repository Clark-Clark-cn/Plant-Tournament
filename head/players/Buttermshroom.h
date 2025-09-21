#pragma once

#include "Player.h"
#include "../bullets/Bubble.h"
#include "../bullets/Butter.h"
#include "../bullets/CopCannon.h"

extern IMAGE img_avatar_buttermshroom;
extern Atlas atlas_buttermshroom_idle;
extern Atlas atlas_buttermshroom_run;
extern Atlas atlas_buttermshroom_attack_ex;
extern Atlas atlas_buttermshroom_die;

extern Audio bubbles_shot;
extern Audio launch;
class Buttermshroom:public Player
{
public:
    Buttermshroom(bool facing_right=true):Player(facing_right)
    {
        animation_idle.setAtlas(&atlas_buttermshroom_idle);
        animation_run.setAtlas(&atlas_buttermshroom_run);
        animation_attack_ex.setAtlas(&atlas_buttermshroom_attack_ex);
        animation_die.setAtlas(&atlas_buttermshroom_die);

        animation_idle.setInterval(75);
        animation_run.setInterval(75);
        animation_attack_ex.setInterval(100);
        animation_die.setInterval(150);

        animation_idle.setLoop(true);
        animation_run.setLoop(true);
        animation_attack_ex.setLoop(true);
        animation_die.setLoop(false);
        statusBar->setAvatar(img_avatar_buttermshroom);
        attack_cd=Config::getInstance()->getInt("player.buttermshroom.attack_cd");
        attack_mp_reward=Config::getInstance()->getInt("player.buttermshroom.attack.mp_reward");
        attackEx_mp_reward=Config::getInstance()->getInt("player.buttermshroom.attackEx.mp_reward");
        damage=Config::getInstance()->getInt("player.buttermshroom.damage");
        Ex_damage=Config::getInstance()->getInt("player.buttermshroom.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Buttermshroom()=default;

    void attack()override{
        bubbles_shot.play();
        Bullet* bullet = new Bubble(this);
        const Vector2 bullet_size=bullet->getSize();
        bullet->setSize(bullet_size);
        Vector2 bullet_position=position+(size-bullet_size)/2;
        bullet->setPosition(bullet_position);
        bullet->setVelocity({0,0});
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=attack_mp_reward;});
        bullet->setDamage(damage);
        bullet_list.push_back(bullet);
        for(int i=0;i<8;i++){
            if(rand()%10!=0)continue;
            Bullet* extra_bullet = new Butter((Butter::Direction)i);
            const Vector2 bullet_size=extra_bullet->getSize();
            const Vector2 bullet_pos={position.x + (size.x - bullet_size.x) / 2,position.y + (size.y - bullet_size.y) / 2};
            extra_bullet->setPosition(bullet_pos);
            extra_bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
            bullet_list.push_back(extra_bullet);
        }
    }

    void attackEx()override{
        launch.play();
        Bullet* bullet = new CopCannon();
        const Vector2 bullet_size=bullet->getSize();
        Vector2 bullet_position=position+(size-bullet_size)/2;
        Vector2 bullet_velocity;
        Vector2 target_pos;
        if(id==PlayerID::P1)target_pos=player_2->getPosition();
        if(id==PlayerID::P2)target_pos=player_1->getPosition();
        bullet->setPosition(bullet_position);
        bullet->setVelocity((target_pos-bullet_position).normalized()*0.5f);
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=attackEx_mp_reward;});
        bullet->setDamage(Ex_damage);
        bullet_list.push_back(bullet);
    }
};
