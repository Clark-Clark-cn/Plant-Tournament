#pragma once

#include "Player.h"
#include "bullets/sunBullet.h"
#include "bullets/sunBulletEx.h"
#include "StatusBar.h"

extern Atlas atlas_sunflower_idle;
extern Atlas atlas_sunflower_run;
extern Atlas atlas_sunflower_attack_ex;
extern Atlas atlas_sunflower_die;
extern Atlas atlas_sun_text;
extern IMAGE img_avatar_sunflower;

extern Player* player_1;
extern Player* player_2;

extern Audio sun_text;
class Sunflower : public Player
{
	Animation animation_sun_text;
	const Vector2 velocity_sun = Config::getInstance()->getVector2("player.sunflower.bullet.velocity");
	const float speed_sun_ex = Config::getInstance()->getFloat("player.sunflower.bulletEx.speed");
	bool is_sun_text_visible = false;
public:
	Sunflower(bool facing_right = true) : Player(facing_right)
	{
		animation_idle.setAtlas(&atlas_sunflower_idle);
		animation_run.setAtlas(&atlas_sunflower_run);
		animation_attack_ex.setAtlas(&atlas_sunflower_attack_ex);
		animation_die.setAtlas(&atlas_sunflower_die);
		animation_sun_text.setAtlas(&atlas_sun_text);

		animation_idle.setInterval(75);
		animation_run.setInterval(75);
		animation_attack_ex.setInterval(100);
		animation_die.setInterval(150);
		animation_sun_text.setInterval(100);

		animation_idle.setLoop(true);
		animation_run.setLoop(true);
		animation_die.setLoop(false);
		animation_attack_ex.setLoop(false);
		animation_sun_text.setLoop(false);

		animation_attack_ex.setCallback([&](){
				is_attacking_ex=false;
				is_sun_text_visible=false;
		});
		statusBar->setAvatar(img_avatar_sunflower);
		attack_cd = Config::getInstance()->getInt("player.sunflower.attack_cd");
		attack_mp_reward = Config::getInstance()->getInt("player.sunflower.attack.mp_reward");
		attackEx_mp_reward = Config::getInstance()->getInt("player.sunflower.attackEx.mp_reward");
		damage = Config::getInstance()->getInt("player.sunflower.damage");
		Ex_damage = Config::getInstance()->getInt("player.sunflower.Exdamage");
        timer_attack_cd.setWaitTime(attack_cd);
	}
	~Sunflower() = default;

	void attack() override
	{
		Bullet* bullet = new SunBullet();
		
		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->getSize();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y;

		bullet->setPosition(bullet_position);
		bullet->setVelocity({is_facing_right?velocity_sun.x:-velocity_sun.x, velocity_sun.y});
		bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
		bullet->setCallback([&] { mp+=attack_mp_reward; });
		bullet->setDamage(damage);
		bullet_list.push_back(bullet);
	}

	void attackEx() override
	{
		sun_text.play();
		is_attacking_ex = true;
		is_sun_text_visible = true;
		animation_sun_text.reset();

		animation_attack_ex.reset();

		Bullet* bullet = new SunBulletEx();
		Vector2 bullet_position, bullet_velocity;
		Player* target = id==PlayerID::P1?player_2:player_1;
		const Vector2& bullet_size = bullet->getSize();
		const Vector2& target_size = target->getSize();
		const Vector2& target_position = target->getPosition();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y=-size.y;
		bullet_velocity.x=0;
		bullet_velocity.y=speed_sun_ex;

		bullet->setPosition(bullet_position);
		bullet->setVelocity({0, -velocity_sun.y});
		bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
		bullet->setCallback([&] { mp+=attackEx_mp_reward; });
		bullet->setDamage(Ex_damage);
		bullet_list.push_back(bullet);
	}

	void update(float delta) override
	{
		Player::update(delta);
		if(is_sun_text_visible)animation_sun_text.update(delta);
	}
	
	void draw(const Camera& camera)override{
		Player::draw(camera);
		if(is_sun_text_visible){
			Vector2 text_position;
			const IMAGE* frame= animation_sun_text.getFrame();
			text_position.x = position.x + (size.x - frame->getWidth()) / 2;
			text_position.y = position.y - frame->getHeight();
			animation_sun_text.draw(camera, (int)text_position.x, (int)text_position.y);
		}
	}
};