#pragma once

#include "Player.h"
#include "../bullets/sunBullet.h"
#include "../bullets/sunBulletEx.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;
extern Atlas atlas_sun_text;
extern IMAGE img_avatar_sunflower;

extern Player* player_1;
extern Player* player_2;

class Sunflower : public Player
{
	Animation animation_sun_text;
	const Vector2 velocity_sun = {0.25f, -0.5f};
	const float speed_sun_ex = 0.15f;
	bool is_sun_text_visible = false;
public:
	Sunflower(bool facing_right = true) : Player(facing_right)
	{
		animation_idle_left.setAtlas(&atlas_sunflower_idle_left);
		animation_idle_right.setAtlas(&atlas_sunflower_idle_right);
		animation_run_left.setAtlas(&atlas_sunflower_run_left);
		animation_run_right.setAtlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.setAtlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.setAtlas(&atlas_sunflower_attack_ex_right);
		animation_die_left.setAtlas(&atlas_sunflower_die_left);
		animation_die_right.setAtlas(&atlas_sunflower_die_right);
		animation_sun_text.setAtlas(&atlas_sun_text);

		animation_idle_left.setInterval(75);
		animation_idle_right.setInterval(75);
		animation_run_left.setInterval(75);
		animation_run_right.setInterval(75);
		animation_attack_ex_left.setInterval(100);
		animation_attack_ex_right.setInterval(100);
		animation_die_left.setInterval(150);
		animation_die_right.setInterval(150);
		animation_sun_text.setInterval(100);

		animation_idle_left.setLoop(true);
		animation_idle_right.setLoop(true);
		animation_run_left.setLoop(true);
		animation_run_right.setLoop(true);
		animation_die_left.setLoop(false);
		animation_die_right.setLoop(false);
		animation_attack_ex_left.setLoop(false);
		animation_attack_ex_right.setLoop(false);
		animation_sun_text.setLoop(false);

		animation_attack_ex_left.setCallback([&](){
				is_attacking_ex=false;
				is_sun_text_visible=false;
		});
		animation_attack_ex_right.setCallback([&](){
				is_attacking_ex=false;
				is_sun_text_visible=false;
		});
		statusBar.setAvatar(&img_avatar_sunflower);
		size.x = 96;
		size.y = 96;
		attack_cd = 250;
	}
	~Sunflower() = default;

	void on_attack() override
	{
		Bullet* bullet = new SunBullet();
		
		Vector2 bullet_position;
		const Vector2& bullet_size = bullet->getSize();
		bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
		bullet_position.y = position.y;

		bullet->setPosition(bullet_position);
		bullet->setVelocity({is_facing_right?velocity_sun.x:-velocity_sun.x, velocity_sun.y});
		bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
		bullet->setCallback([&] { mp+=35; });
		bullet_list.push_back(bullet);
	}

	void on_attack_ex() override
	{
		mciSendString(L"play sun_text from 0", NULL, 0, NULL);
		is_attacking_ex = true;
		is_sun_text_visible = true;
		animation_sun_text.reset();

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new SunBulletEx();
		Vector2 bullet_position, bullet_velocity;
		Player* target = id==PlayerID::P1?player_2:player_1;
		const Vector2& bullet_size = bullet->getSize();
		const Vector2& target_size = target->get_size();
		const Vector2& target_position = target->get_position();
		bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
		bullet_position.y=-size.y;
		bullet_velocity.x=0;
		bullet_velocity.y=speed_sun_ex;

		bullet->setPosition(bullet_position);
		bullet->setVelocity({0, -velocity_sun.y});
		bullet->setCollideTarget(id==PlayerID::P1?PlayerID::P2:PlayerID::P1);
		bullet->setCallback([&] { mp+=50; });
		bullet_list.push_back(bullet);
	}

	void update(int delta) override
	{
		Player::update(delta);
		if(is_sun_text_visible)animation_sun_text.update(delta);
	}
	
	void draw(const Camera& camera){
		Player::draw(camera);
		if(is_sun_text_visible){
			Vector2 text_position;
			IMAGE* frame= animation_sun_text.getFrame();
			text_position.x = position.x + (size.x - frame->getwidth()) / 2;
			text_position.y = position.y - frame->getheight();
			animation_sun_text.draw(camera, (int)text_position.x, (int)text_position.y);
		}
	}
};