#pragma once
#include "baseItem/Timer.h"
#include "Player.h"
#include "baseItem/Camera.h"
#include "bullets/peaBullet.h"
#include "StatusBar.h"

extern Atlas atlas_peashooter_idle;
extern Atlas atlas_peashooter_run;
extern Atlas atlas_peashooter_attack_ex;
extern Atlas atlas_peashooter_die;
extern IMAGE img_avatar_peashooter;

extern Camera camera;

extern Audio pea_shoot[2];
extern Audio pea_shoot_ex;
class Peashooter : public Player
{
	Timer timer_attack_ex;
	Timer timer_spawn_pea_ex;

public:
	Peashooter(bool facing_right = true) : Player(facing_right)
	{
		animation_idle.setAtlas(&atlas_peashooter_idle);
		animation_run.setAtlas(&atlas_peashooter_run);
		animation_attack_ex.setAtlas(&atlas_peashooter_attack_ex);
		animation_die.setAtlas(&atlas_peashooter_die);

		animation_idle.setInterval(75);
		animation_run.setInterval(75);
		animation_attack_ex.setInterval(75);
		animation_die.setInterval(75);

		animation_idle.setLoop(true);
		animation_run.setLoop(true);
		animation_die.setLoop(false);

		timer_attack_ex.setWaitTime(attack_ex_duration);
		timer_attack_ex.setOneShot(true);
		timer_attack_ex.set_callback([&](){ is_attacking_ex = false; });
		timer_spawn_pea_ex.setWaitTime(100);
		timer_spawn_pea_ex.set_callback([&](){ spawn_pea_bullet(speed_pea_ex); });

		statusBar->setAvatar(img_avatar_peashooter);
		attack_cd = Config::getInstance()->getInt("player.peashooter.attack_cd");
		attack_mp_reward = Config::getInstance()->getInt("player.peashooter.attack.mp_reward");
		damage = Config::getInstance()->getInt("player.peashooter.damage");
        timer_attack_cd.setWaitTime(attack_cd);
	}
	~Peashooter() = default;

	void attack() override
	{
		pea_shoot[rand() % 2].play();
		spawn_pea_bullet(speed_pea);
	}

	void attackEx() override
	{
		pea_shoot_ex.play();
		is_attacking_ex = true;
		timer_attack_ex.restart();

		animation_attack_ex.reset();
	}

	void update(float delta) override
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
	const float speed_pea = Config::getInstance()->getFloat("player.peashooter.pea.speed");
	const float speed_pea_ex = Config::getInstance()->getFloat("player.peashooter.peaEx.speed");
	const int attack_ex_duration = Config::getInstance()->getInt("player.peashooter.peaEx.duration");
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
		bullet->setDamage(damage);

		bullet->setCollideTarget(id == PlayerID::P1 ? PlayerID::P2 : PlayerID::P1);
		bullet->setCallback([&](){mp+=attack_mp_reward;});
		bullet_list.push_back(bullet);
	}
};
