#pragma once

#include "../Timer.h"
#include "../bullets/Bullet.h"
#include "../Vector2.h"
#include "../platform.h"
#include "../Particle.h"
#include "player_id.h"

#include <Windows.h>
#include <vector>
#include <graphics.h>

class StatusBar;

class Player
{
	friend class StatusBar;
public:
	Player(bool facing_right = true);
	~Player() = default;

	virtual void update(int delta);

	virtual void draw(const Camera& camera);
	virtual void input(const ExMessage& msg);
	

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

	const Vector2& getPosition() const
	{
		return position;
	}

	const Vector2& getSize() const
	{
		return size;
	}
protected:
	const float gravity = 1.6e-3f;
	const float run_velocity = 0.55f;
	const float jump_velocity = -0.85f;

protected:
	float mp = 0;
	float hp = 100.0f;
	int attack_cd = 500;
	IMAGE img_sketch;
	PlayerID id = PlayerID::P1;
	Vector2 size;
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
	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;
	Animation animation_die_left;
	Animation animation_die_right;
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

	void randomSummonEffortBullets();
};