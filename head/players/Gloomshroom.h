#include "Player.h"
#include "../bullets/Bubble.h"
#include "../bullets/BubbleEx.h"
#include "../StatusBar.h"

extern IMAGE img_avatar_gloomshroom;
extern Atlas atlas_gloomshroom_idle_left;
extern Atlas atlas_gloomshroom_idle_right;
extern Atlas atlas_gloomshroom_run_left;
extern Atlas atlas_gloomshroom_run_right;
extern Atlas atlas_gloomshroom_attack_ex_left;
extern Atlas atlas_gloomshroom_attack_ex_right;
extern Atlas atlas_gloomshroom_die_left;
extern Atlas atlas_gloomshroom_die_right;

class Gloomshroom:public Player
{
public:
    Gloomshroom(bool facing_right=true):Player(facing_right)
    {
        animation_idle_left.setAtlas(&atlas_gloomshroom_idle_left);
        animation_idle_right.setAtlas(&atlas_gloomshroom_idle_right);
        animation_run_left.setAtlas(&atlas_gloomshroom_run_left);
        animation_run_right.setAtlas(&atlas_gloomshroom_run_right);
        animation_attack_ex_left.setAtlas(&atlas_gloomshroom_attack_ex_left);
        animation_attack_ex_right.setAtlas(&atlas_gloomshroom_attack_ex_right);
        animation_die_left.setAtlas(&atlas_gloomshroom_die_left);
        animation_die_right.setAtlas(&atlas_gloomshroom_die_right);

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

        timer_attack_ex.setWaitTime(attack_ex_duration);
        timer_attack_ex.setOneShot(true);
        timer_attack_ex.set_callback([&](){
            is_attacking_ex=false;
        });
        statusBar->setAvatar(&img_avatar_gloomshroom);
        size.x=96;
        size.y=96;
        attack_cd=700;
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Gloomshroom()=default;

    void attack() override{
        mciSendString(L"play bubbles_shot from 0", NULL, 0, NULL);
        Bullet* bullet = new Bubble(this);

        const Vector2& bullet_size = bullet->getSize();
        Vector2 bullet_position=position+(size-bullet_size)/2;
        bullet->setPosition(bullet_position);
        bullet->setVelocity({0,0});
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=35;});
        bullet_list.push_back(bullet);
    }
    void attackEx() override{
        mciSendString(L"play bubbles_ex_shot from 0", NULL, 0, NULL);
        is_attacking_ex=true;
        timer_attack_ex.restart();
        Bullet* bullet = new BubbleEx(this);
        const Vector2& bullet_size = bullet->getSize();
        Vector2 bullet_position=position-bullet_size/2;
        bullet->setPosition(bullet_position);
        bullet->setVelocity({0,0});
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=10;});
        bullet_list.push_back(bullet);
    }
    void update(int delta) override{
        Player::update(delta);
        if(is_attacking_ex)timer_attack_ex.update(delta);
    }
private:
    const int attack_ex_duration=3000;
    Timer timer_attack_ex;
};
