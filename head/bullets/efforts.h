#pragma once

#include "Bullet.h"
#include "players/Player.h"

extern Atlas atlas_buff_box_blue;
extern Atlas atlas_buff_box_pink;
extern Atlas atlas_buff_box_yellow;

class EffortBullet : public Bullet
{
protected:
	enum class effortType{
		recover,
		hurry,
		invisible,
		silence
	};
	effortType type;
	Animation animation;
public:
	EffortBullet()
	{
		velocity = Vector2(0, -0.2);
		type = static_cast<effortType>(rand() % 4);
		size = Vector2(80, 80);
		switch(rand() % 3){
			case 0:
				animation.setAtlas(&atlas_buff_box_blue);
				break;
			case 1:
				animation.setAtlas(&atlas_buff_box_pink);
				break;
			case 2:
				animation.setAtlas(&atlas_buff_box_yellow);
				break;
		}
		animation.setInterval(100);
		animation.setLoop(true);
		position.x=rand()%WINDOW_WIDTH;
		position.y=WINDOW_HEIGHT;
	}
	~EffortBullet() = default;

	void update(float delta) override
	{
		position += velocity * (float)delta;
		animation.update(delta);
		if (checkIfExceedsScreen()) canRemove = true;
	}
	void draw(const Camera& camera) const override
	{
		animation.draw(camera, position.x, position.y);
		Bullet::draw(camera);
	}
	void collide(Player* target)
	{
		Bullet::collide();
		switch (type)
		{
		case effortType::recover:
			target->set_recovering(true);
			break;
		case effortType::hurry:
			target->set_hurrying(true);
			break;
		case effortType::invisible:
			target->set_invisible(true);
			break;
		case effortType::silence:
			target->set_silenced(true);
			break;
		}
		isValid = false;
		canRemove = true;
	}
};
