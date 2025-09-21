#pragma once

#include "baseItem/Timer.h"
#include "bullets/Bullet.h"
#include "baseItem/Vector2.h"
#include "platform.h"
#include "Particle.h"
#include "baseItem/Config.h"
#include "player_id.h"

#include <vector>

class StatusBar;
class EffortBullet;

class Player
{
	friend class StatusBar;
	friend class EffortBullet;
public:
	Player(bool facing_right = true);
	~Player() = default;

	virtual void update(float delta);

	virtual void draw(const Camera& camera);
	virtual void input(const SDL_Event& msg);
	

	virtual void on_run(float dir_x);

	virtual void on_jump();

	virtual void on_land();
	virtual void attack() {}
	virtual void attackEx() {}

	void set_buttered(bool val)
	{
		is_buttered = val;
		if (is_buttered)
			timer_butter.restart();
		is_left_key_down = false;
		is_right_key_down = false;
	}
	void set_hp(int val)
	{
		hp = val;
	}

	int get_hp() const
	{
		return hp;
	}

	int get_mp() const
	{
		return mp;
	}

	void set_id(PlayerID id);

	void set_position(float x, float y)
	{
		position.x = x; position.y = y;
	}

	void set_silenced(bool val)
	{
		is_silenced = val;
		if (is_silenced)
			timer_silence.restart();
	}

	void set_invisible(bool val)
	{
		is_invisible = val;
		if (is_invisible)
			timer_invisible.restart();
	}

	void set_hurrying(bool val)
	{
		is_hurrying = val;
		if (is_hurrying)
			timer_hurry.restart();
	}

	void set_recovering(bool val)
	{
		is_recovering = val;
		if (is_recovering)
			timer_recover.restart();
	}

	const Vector2& getPosition() const
	{
		return position;
	}

	const Vector2& getSize() const
	{
		return size;
	}
protected:
	const float gravity=Config::getInstance()->getFloat("player.gravity");
	const float run_velocity=Config::getInstance()->getFloat("player.run_velocity");
	const float jump_velocity=Config::getInstance()->getFloat("player.jump_velocity");
protected:
	float mp = 0;
	float hp=Config::getInstance()->getFloat("player.max_hp");
	float max_hp=Config::getInstance()->getFloat("player.max_hp");
	float hurry_multiplier=Config::getInstance()->getFloat("player.multiplier.hurrying");
	float recover_multiplier=Config::getInstance()->getFloat("player.multiplier.recovery");
	int attack_cd=Config::getInstance()->getInt("player.default.attack_cd");
	int damage=Config::getInstance()->getInt("player.default.damage");
	int Ex_damage=Config::getInstance()->getInt("player.default.Exdamage");
	int attack_mp_reward=Config::getInstance()->getInt("player.default.attack.mp_reward");
	int attackEx_mp_reward=Config::getInstance()->getInt("player.default.attackEx.mp_reward");
	IMAGE img_sketch;
	PlayerID id = PlayerID::P1;
	Vector2 size=Config::getInstance()->getVector2("player.size");
	Vector2 position;
	Vector2 velocity;
	Vector2 position_jump_effect;
	Vector2 position_land_effect;
	Vector2 last_hurt_direction;
	bool can_attack = true;
	bool is_facing_right = true;
	bool is_cursor_visible = true;
	bool is_jump_effect_visible = false;
	bool is_land_effect_visible = false;
	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_attacking_ex = false;
	bool is_invulnerable = false;
	bool is_showing_sketch_frame = false;
	bool is_silenced = false;
	bool is_recovering = false;
	bool is_hurrying = false;
	bool is_invisible = false;
	bool is_buttered = false;
	Animation animation_idle;
	Animation animation_run;
	Animation animation_attack_ex;
	Animation animation_die;
	Animation animation_jump_effect;
	Animation animation_land_effect;
	Animation* current_animation = nullptr;
	Timer timer_attack_cd;
	Timer timer_invulnerable;
	Timer timer_invulnerable_blink;
	Timer timer_cursor_visibility;
	Timer timer_run_effect_generation;
	Timer timer_die_effect_generation;
	Timer timer_silence;
	Timer timer_recover;
	Timer timer_hurry;
	Timer timer_invisible;
	Timer timer_butter;
	std::vector<Particle> particle_list;

	std::shared_ptr<StatusBar> statusBar;
protected:
	void make_invulnerable()
	{
		is_invulnerable = true;
		timer_invulnerable.restart();
	}

	void move_and_collide(int delta);
};