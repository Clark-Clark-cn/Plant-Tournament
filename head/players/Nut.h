#include "Player.h"
#include "../bullets/NutExplode.h"
#include "../StatusBar.h"

extern IMAGE img_avatar_nut;
extern Atlas atlas_nut_idle;
extern Atlas atlas_nut_run;
extern Atlas atlas_nut_attack_ex;
extern Atlas atlas_nut_die;

extern Audio nut_explode;
extern Audio nut_dash;

class Nut:public Player
{
public:
    Nut(bool facing_right=true):Player(facing_right),
        dash_distance(Config::getInstance()->getInt("player.nut.dash_distance"))
    {
        animation_idle.setAtlas(&atlas_nut_idle);
        animation_run.setAtlas(&atlas_nut_run);
        animation_attack_ex.setAtlas(&atlas_nut_attack_ex);
        animation_die.setAtlas(&atlas_nut_die);

        animation_idle.setInterval(75);
        animation_run.setInterval(75);
        animation_attack_ex.setInterval(100);
        animation_die.setInterval(150);

        animation_idle.setLoop(true);
        animation_run.setLoop(true);
        animation_attack_ex.setLoop(false);
        animation_die.setLoop(false);

        animation_attack_ex.setCallback([&](){
            SummonBoom();
            is_attacking_ex=false;
        });

        statusBar->setAvatar(img_avatar_nut);
        attack_cd=Config::getInstance()->getInt("player.nut.attack_cd");
        attackEx_mp_reward=Config::getInstance()->getInt("player.nut.attackEx.mp_reward");
        Ex_damage=Config::getInstance()->getInt("player.nut.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Nut()=default;
    void attack()override{
        nut_dash.play();
        if(is_facing_right)
            position.x+=dash_distance;
        else
            position.x-=dash_distance;
    }
    void attackEx()override{
        nut_explode.play();
        is_attacking_ex=true;
    }
    void update(float delta)override{
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