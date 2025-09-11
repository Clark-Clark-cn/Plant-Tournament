#ifndef _PEASHOER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include "../Timer.h"
#include "Player.h"
#include "../Camera.h"
#include "../bullets/peaBullet.h"
#include "../StatusBar.h"

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;
extern Atlas atlas_peashooter_die_right;
extern IMAGE img_avatar_peashooter;

extern Camera camera;

class Peashooter : public Player
{
	Timer timer_attack_ex;
	Timer timer_spawn_pea_ex;

public:
	Peashooter(bool facing_right = true) : Player(facing_right)
	{
		animation_idle_left.setAtlas(&atlas_peashooter_idle_left);
		animation_idle_right.setAtlas(&atlas_peashooter_idle_right);
		animation_run_left.setAtlas(&atlas_peashooter_run_left);
		animation_run_right.setAtlas(&atlas_peashooter_run_right);
		animation_attack_ex_left.setAtlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.setAtlas(&atlas_peashooter_attack_ex_right);
		animation_die_left.setAtlas(&atlas_peashooter_die_left);
		animation_die_right.setAtlas(&atlas_peashooter_die_right);

		animation_idle_left.setInterval(75);
		animation_idle_right.setInterval(75);
		animation_run_left.setInterval(75);
		animation_run_right.setInterval(75);
		animation_attack_ex_left.setInterval(75);
		animation_attack_ex_right.setInterval(75);
		animation_die_left.setInterval(75);
		animation_die_right.setInterval(75);

		animation_idle_left.setLoop(true);
		animation_idle_right.setLoop(true);
		animation_run_left.setLoop(true);
		animation_run_right.setLoop(true);
		animation_die_left.setLoop(false);
		animation_die_right.setLoop(false);

		timer_attack_ex.setWaitTime(attack_ex_duration);
		timer_attack_ex.setOneShot(true);
		timer_attack_ex.set_callback([&](){ is_attacking_ex = false; });
		timer_spawn_pea_ex.setWaitTime(100);
		timer_spawn_pea_ex.set_callback([&](){ spawn_pea_bullet(speed_pea_ex); });

		statusBar->setAvatar(&img_avatar_peashooter);
		size.x = 96;
		size.y = 96;
		attack_cd = 100;
        timer_attack_cd.setWaitTime(attack_cd);
	}
	~Peashooter() = default;

	void attack() override
	{
		std::wstring cmd=L"play pea_shoot_"+std::to_wstring(rand() % 2) + L" from 0";
		mciSendString(cmd.c_str(), NULL, 0, NULL);

		spawn_pea_bullet(speed_pea);
	}

	void attackEx() override
	{
		mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
		is_attacking_ex = true;
		timer_attack_ex.restart();

		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
	}

	void update(int delta) override
	{
		Player::update(delta);

		if (is_attacking_ex)
		{
			camera.shake(5, 100);
			timer_attack_ex.update(delta);
			timer_spawn_pea_ex.update(delta);
		}
	}

private:
	const float speed_pea = 0.75f;
	const float speed_pea_ex = 1.5f;
	const int attack_ex_duration = 2500;
	void spawn_pea_bullet(float speed)
	{
		Bullet *bullet = new PeaBullet();

		Vector2 bullet_position, bullet_velocity;
		const Vector2 &bullet_size = bullet->getSize();
		bullet_position.x=is_facing_right ? position.x + size.x - bullet_size.x / 2 : position.x - bullet_size.x / 2;
		bullet_position.y=position.y;
		bullet_velocity.x=is_facing_right ? speed : -speed;
		bullet_velocity.y=0;

		bullet->setPosition(bullet_position);
		bullet->setVelocity(bullet_velocity);

		bullet->setCollideTarget(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		bullet->setCallback([&](){mp+=25;});
		bullet_list.push_back(bullet);
	}
};

#endif // _PEASHOOTER_PLAYER_H_
