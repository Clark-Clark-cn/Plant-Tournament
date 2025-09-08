#pragma once

#include "Bullet.h"
#include "../Camera.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

class SunBullet : public Bullet
{
public:
    SunBullet()
    {
        size.x = 96;
        size.y = 96;
        damage = 20;

        animation_idle.setAtlas(&atlas_sun);
        animation_idle.setInterval(50);
        animation_idle.setLoop(true);

        animation_break.setAtlas(&atlas_sun_explode);
        animation_break.setInterval(100);
        animation_break.setLoop(false);
        animation_break.setCallback([&] { canRemove = true; });
        IMAGE* frameIdle = animation_idle.getFrame();
        IMAGE* frameBreak = animation_break.getFrame();
        breakRenderOffset.x = (frameIdle->getwidth() - frameBreak->getwidth()) / 2;
        breakRenderOffset.y = (frameIdle->getheight() - frameBreak->getheight()) / 2;
    }
    ~SunBullet() = default;

    void collide() override
    {
        mciSendString(L"play sun_explode from 0", nullptr, 0, nullptr);
        Bullet::collide();
        camera.shake(5,250);
    }

    void update(int delta) override
    {
        if (isValid)
        {
            velocity.y += gravity * delta;
            position += velocity * (float)delta;
        }
        if (!isValid) animation_break.update(delta);
        else animation_idle.update(delta);
        if (checkIfExceedsScreen()) canRemove = true;
    }

    void draw(const Camera& camera)const override{
        if (isValid) animation_idle.draw(camera, position.x, position.y);
        else {
            animation_break.draw(camera, position.x + breakRenderOffset.x, position.y + breakRenderOffset.y);
        }
        Bullet::draw(camera);
    }
private:
    Vector2 direction;
    Animation animation_break;
    Animation animation_idle;
    Vector2 breakRenderOffset;
    const float gravity = 1e-3f;
};
