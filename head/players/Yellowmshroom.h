#pragma once

#include "Player.h"
#include "../bullets/Bubble.h"
#include "../bullets/Butter.h"
#include "../bullets/CopCannon.h"

extern IMAGE img_avatar_yellowmshroom;
extern Atlas atlas_yellowmshroom_idle_left;
extern Atlas atlas_yellowmshroom_idle_right;
extern Atlas atlas_yellowmshroom_run_left;
extern Atlas atlas_yellowmshroom_run_right;
extern Atlas atlas_yellowmshroom_attack_ex_left;
extern Atlas atlas_yellowmshroom_attack_ex_right;
extern Atlas atlas_yellowmshroom_die_left;
extern Atlas atlas_yellowmshroom_die_right;

class Yellowmshroom:public Player
{
public:
    Yellowmshroom(bool facing_right=true):Player(facing_right)
    {
        animation_idle_left.setAtlas(&atlas_yellowmshroom_idle_left);
        animation_idle_right.setAtlas(&atlas_yellowmshroom_idle_right);
        animation_run_left.setAtlas(&atlas_yellowmshroom_run_left);
        animation_run_right.setAtlas(&atlas_yellowmshroom_run_right);
        animation_attack_ex_left.setAtlas(&atlas_yellowmshroom_attack_ex_left);
        animation_attack_ex_right.setAtlas(&atlas_yellowmshroom_attack_ex_right);
        animation_die_left.setAtlas(&atlas_yellowmshroom_die_left);
        animation_die_right.setAtlas(&atlas_yellowmshroom_die_right);

        animation_idle_left.setInterval(75);
        animation_idle_right.setInterval(75);
        animation_run_left.setInterval(75);
        animation_run_right.setInterval(75);
        animation_attack_ex_left.setInterval(100);
        animation_attack_ex_right.setInterval(100);
        animation_die_left.setInterval(150);
        animation_die_right.setInterval(150);

        animation_idle_left.setLoop(true);
        animation_idle_right.setLoop(true);
        animation_run_left.setLoop(true);
        animation_run_right.setLoop(true);
        animation_attack_ex_left.setLoop(true);
        animation_attack_ex_right.setLoop(true);
        animation_die_left.setLoop(false);
        animation_die_right.setLoop(false);
        statusBar->setAvatar(&img_avatar_yellowmshroom);
        attack_cd=Config::getInstance()->getInt("player.yellowmshroom.attack_cd");
        attack_mp_reward=Config::getInstance()->getInt("player.yellowmshroom.attack.mp_reward");
        attackEx_mp_reward=Config::getInstance()->getInt("player.yellowmshroom.attackEx.mp_reward");
        damage=Config::getInstance()->getInt("player.yellowmshroom.damage");
        Ex_damage=Config::getInstance()->getInt("player.yellowmshroom.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Yellowmshroom()=default;

    void attack()override{
        mciSendString(L"play bubbles_shot from 0", NULL, 0, NULL);
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
        mciSendString(L"play launch from 0", NULL, 0, NULL);
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
