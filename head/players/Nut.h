#include "Player.h"
#include "../bullets/NutExplode.h"
#include "../StatusBar.h"

extern IMAGE img_avatar_nut;
extern Atlas atlas_nut_idle_left;
extern Atlas atlas_nut_idle_right;
extern Atlas atlas_nut_run_left;
extern Atlas atlas_nut_run_right;
extern Atlas atlas_nut_attack_ex_left;
extern Atlas atlas_nut_attack_ex_right;
extern Atlas atlas_nut_die_left;
extern Atlas atlas_nut_die_right;

class Nut:public Player
{
public:
    Nut(bool facing_right=true):Player(facing_right),
        dash_distance(Config::getInstance()->getInt("player.nut.dash_distance"))
    {
        animation_idle_left.setAtlas(&atlas_nut_idle_left);
        animation_idle_right.setAtlas(&atlas_nut_idle_right);
        animation_run_left.setAtlas(&atlas_nut_run_left);
        animation_run_right.setAtlas(&atlas_nut_run_right);
        animation_attack_ex_left.setAtlas(&atlas_nut_attack_ex_left);
        animation_attack_ex_right.setAtlas(&atlas_nut_attack_ex_right);
        animation_die_left.setAtlas(&atlas_nut_die_left);
        animation_die_right.setAtlas(&atlas_nut_die_right);

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
        animation_attack_ex_left.setLoop(false);
        animation_attack_ex_right.setLoop(false);
        animation_die_left.setLoop(false);
        animation_die_right.setLoop(false);

        animation_attack_ex_left.setCallback([&](){
            SummonBoom();
            is_attacking_ex=false;
        });

        animation_attack_ex_right.setCallback([&](){
            SummonBoom();
            is_attacking_ex=false;
        });

        statusBar->setAvatar(&img_avatar_nut);
        attack_cd=Config::getInstance()->getInt("player.nut.attack_cd");
        attackEx_mp_reward=Config::getInstance()->getInt("player.nut.attackEx.mp_reward");
        Ex_damage=Config::getInstance()->getInt("player.nut.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Nut()=default;
    void attack()override{
        mciSendString(L"play nut_dash from 0", NULL, 0, NULL);
        if(is_facing_right)
            position.x+=dash_distance;
        else
            position.x-=dash_distance;
    }
    void attackEx()override{
        mciSendString(L"play nut_explode from 0", NULL, 0, NULL);
        is_attacking_ex=true;
    }
    void update(int delta)override{
        Player::update(delta);
        if(hp<max_hp)hp+=delta/100.0f;
        if(hp>max_hp)hp=max_hp;
        if(mp<100)mp+=delta/150.0f;
        if(mp>100)mp=100.0f;
    }
private:
    const int dash_distance;
    void SummonBoom(){
        Bullet* boom = new NutExplode();
        const Vector2& boom_size = boom->getSize();
        Vector2 boom_position=position-(boom_size-size)/2;
        boom->setPosition(boom_position);
        boom->setVelocity({0,0});
        boom->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        boom->setCallback([&]{mp+=attackEx_mp_reward;});
        boom->setDamage(Ex_damage);
        bullet_list.push_back(boom);
    }
};