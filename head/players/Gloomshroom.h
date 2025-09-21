#include "Player.h"
#include "../bullets/Bubble.h"
#include "../bullets/BubbleEx.h"
#include "../StatusBar.h"

extern IMAGE img_avatar_gloomshroom;
extern Atlas atlas_gloomshroom_idle;
extern Atlas atlas_gloomshroom_run;
extern Atlas atlas_gloomshroom_attack_ex;
extern Atlas atlas_gloomshroom_die;

extern Audio bubbles_shot;
extern Audio bubbles_shot_ex;

class Gloomshroom:public Player
{
public:
    Gloomshroom(bool facing_right=true):Player(facing_right)
    {
        animation_idle.setAtlas(&atlas_gloomshroom_idle);
        animation_run.setAtlas(&atlas_gloomshroom_run);
        animation_attack_ex.setAtlas(&atlas_gloomshroom_attack_ex);
        animation_die.setAtlas(&atlas_gloomshroom_die);

        animation_idle.setInterval(75);
        animation_run.setInterval(75);
        animation_attack_ex.setInterval(100);
        animation_die.setInterval(150);

        animation_idle.setLoop(true);
        animation_run.setLoop(true);
        animation_attack_ex.setLoop(true);
        animation_die.setLoop(false);

        timer_attack_ex.setWaitTime(attack_ex_duration);
        timer_attack_ex.setOneShot(true);
        timer_attack_ex.set_callback([&](){
            is_attacking_ex=false;
        });
        statusBar->setAvatar(img_avatar_gloomshroom);
        attack_cd=Config::getInstance()->getInt("player.gloomshroom.attack_cd");
        attack_mp_reward=Config::getInstance()->getInt("player.gloomshroom.attack.mp_reward");
        attackEx_mp_reward=Config::getInstance()->getInt("player.gloomshroom.attackEx.mp_reward");
        damage=Config::getInstance()->getInt("player.gloomshroom.damage");
        Ex_damage=Config::getInstance()->getInt("player.gloomshroom.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
    }
    ~Gloomshroom()=default;

    void attack() override{
        bubbles_shot.play();
        Bullet* bullet = new Bubble(this);

        const Vector2& bullet_size = bullet->getSize();
        Vector2 bullet_position=position+(size-bullet_size)/2;
        bullet->setPosition(bullet_position);
        bullet->setVelocity({0,0});
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=attack_mp_reward;});
        bullet->setDamage(damage);
        bullet_list.push_back(bullet);
    }
    void attackEx() override{
        bubbles_shot_ex.play();
        is_attacking_ex=true;
        timer_attack_ex.restart();
        Bullet* bullet = new BubbleEx(this);
        const Vector2& bullet_size = bullet->getSize();
        Vector2 bullet_position=position-bullet_size/2;
        bullet->setPosition(bullet_position);
        bullet->setVelocity({0,0});
        bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
        bullet->setCallback([&]{mp+=attackEx_mp_reward;});
        bullet->setDamage(Ex_damage);
        bullet_list.push_back(bullet);
    }
    void update(float delta) override{
        Player::update(delta);
        if(is_attacking_ex)timer_attack_ex.update(delta);
    }
private:
    const int attack_ex_duration=3000;
    Timer timer_attack_ex;
};
