#include "head/players/Player.h"
#include "head/StatusBar.h"

extern IMAGE img_1p_cursor;
extern IMAGE img_2p_cursor;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_butter_splat;

extern std::vector<Bullet*> bullet_list;
extern std::vector<Bullet*> effort_bullets;
extern std::vector<Platform> platform_list;

extern bool is_debug;


Player::Player(bool facing_right)
     : is_facing_right(facing_right), statusBar(std::make_shared<StatusBar>(this))
{
    animation_jump_effect.setAtlas(&atlas_jump_effect);
    animation_jump_effect.setInterval(25);
    animation_jump_effect.setLoop(false);
    animation_jump_effect.setCallback([&]()
        {
            is_jump_effect_visible = false;
        }
    );

    animation_land_effect.setAtlas(&atlas_land_effect);
    animation_land_effect.setInterval(50);
    animation_land_effect.setLoop(false);
    animation_land_effect.setCallback([&]()
        {
            is_land_effect_visible = false;
        }
    );

    current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;

    timer_invulnerable.setWaitTime(750);
    timer_invulnerable.setOneShot(true);
    timer_invulnerable.set_callback([&]()
        {
            is_invulnerable = false;
        }
    );

    timer_invulnerable_blink.setWaitTime(75);
    timer_invulnerable_blink.set_callback([&]()
        {
            is_showing_sketch_frame = !is_showing_sketch_frame;
        }
    );

    timer_cursor_visibility.setWaitTime(2500);
    timer_cursor_visibility.setOneShot(true);
    timer_cursor_visibility.set_callback([&]()
        {
            is_cursor_visible = false;
        }
    );

    timer_run_effect_generation.setWaitTime(75);
    timer_run_effect_generation.set_callback([&]()
        {
            Vector2 particle_position;
            IMAGE* frame = atlas_run_effect.getImage(0);
            particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
            particle_position.y = position.y + size.y - frame->getheight();
            particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
        }
    );

    timer_die_effect_generation.setWaitTime(35);
    timer_die_effect_generation.set_callback([&]()
        {
            Vector2 particle_position;
            IMAGE* frame = atlas_run_effect.getImage(0);
            particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
            particle_position.y = position.y + size.y - frame->getheight();
            particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
        }
    );

    timer_attack_cd.setOneShot(true);
    timer_attack_cd.set_callback([&]()
        {
            can_attack = true;
        }
    );
    timer_butter.setWaitTime(3000);
    timer_butter.setOneShot(true);
    timer_butter.set_callback([&]()
        {
            is_buttered = false;
        }
    );
}

void Player::update(int delta)
{
    int direction = is_right_key_down - is_left_key_down;

    if (direction != 0)
    {
        if (!is_attacking_ex)
        {
            is_facing_right = direction > 0;
            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
            on_run(direction * run_velocity * delta);
        }
    }
    else
    {
        current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        timer_run_effect_generation.pause();
    }

    if (is_attacking_ex)
    {
        current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
    }

    if (hp <= 0)
    {
        current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;
    }

    if(!is_buttered)current_animation->update(delta);
    animation_jump_effect.update(delta);
    animation_land_effect.update(delta);

    timer_attack_cd.update(delta);
    timer_invulnerable.update(delta);
    timer_invulnerable_blink.update(delta);
    timer_cursor_visibility.update(delta);
    timer_run_effect_generation.update(delta);

    if (hp <= 0)
    {
        timer_die_effect_generation.update(delta);
    }

    if (is_showing_sketch_frame)
    {
        sketch_image(current_animation->getFrame(), &img_sketch);
    }

    particle_list.erase(std::remove_if(
        particle_list.begin(), particle_list.end(),
        [](const Particle& particle)
        {
            return !particle.checkValid();
        }
    ), particle_list.end());

    for (Particle& particle : particle_list)
    {
        particle.update(delta);
    }

    move_and_collide(delta);

    if(position.y>getwidth())hp=0;
    randomSummonEffortBullets();
    
    if(is_silenced)timer_silence.update(delta);
    if(is_recovering)timer_recover.update(delta);
    if(is_hurrying)timer_hurry.update(delta);
    if(is_invisible)timer_invisible.update(delta);
    if(is_buttered)timer_butter.update(delta);
}

void Player::draw(const Camera& camera)
{
    if (is_jump_effect_visible)
    {
        animation_jump_effect.draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
    }

    if (is_land_effect_visible)
    {
        animation_land_effect.draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);
    }

    for (const Particle& particle : particle_list)
    {
        particle.draw(camera);
    }

    if (hp > 0 && is_invulnerable && is_showing_sketch_frame)
    {
        putImage(camera, position.x, position.y, &img_sketch);
    }
    else
    {
        current_animation->draw(camera, (int)position.x, (int)position.y);
    }
    statusBar->draw();

    if(is_buttered){
        putImage(camera,position.x+size.x/4,position.y,&img_butter_splat);
    }

    if (is_debug)
    {
        setlinecolor(RGB(0, 125, 255));
        rectangle((int)position.x, (int)position.y, (int)position.x + size.x, (int)position.y + size.y);
    }

    if (is_cursor_visible){
        switch (id)
        {
        case PlayerID::P1:
            putImage(camera, (int)position.x + (size.x - img_1p_cursor.getwidth()) / 2, (int)position.y - img_1p_cursor.getheight(), &img_1p_cursor);
            break;
        case PlayerID::P2:
            putImage(camera, (int)position.x + (size.x - img_2p_cursor.getwidth()) / 2, (int)position.y - img_2p_cursor.getheight(), &img_2p_cursor);
            break;
        }
    }
}

void Player::input(const ExMessage& msg){
    if (hp <= 0)
        return;
    if(is_silenced)
        return;
    if(is_buttered)
        return;

    switch (msg.message)
    {
    case WM_KEYDOWN:
        switch (id)
        {
        case PlayerID::P1:
            switch (msg.vkcode)
            {
            case 'A':
                is_left_key_down = true;
                break;
            case 'D':
                is_right_key_down = true;
                break;
            case 'W':
                on_jump();
                break;
            case 'F':
                if (can_attack)
                {
                    attack();
                    can_attack = false;
                    timer_attack_cd.restart();
                }
                break;
            case 'G':
                if (mp >= 100)
                {
                    attackEx();
                    mp = 0;
                }
                break;
            }
            break;
        case PlayerID::P2:
            switch (msg.vkcode)
            {
            case VK_LEFT:
                is_left_key_down = true;
                break;
            case VK_RIGHT:
                is_right_key_down = true;
                break;
            case VK_UP:
                on_jump();
                break;
            case VK_OEM_PERIOD:
                if (can_attack)
                {
                    attack();
                    can_attack = false;
                    timer_attack_cd.restart();
                }
                break;
            case VK_OEM_2:
                if (mp >= 100)
                {
                    attackEx();
                    mp = 0;
                }
                break;
            }
            break;
        }
        break;
    case WM_KEYUP:
        switch (id)
        {
        case PlayerID::P1:
            switch (msg.vkcode)
            {
            case 'A':
                is_left_key_down = false;
                break;
            case 'D':
                is_right_key_down = false;
                break;
            }
            break;
        case PlayerID::P2:
            switch (msg.vkcode)
            {
            case VK_LEFT:
                is_left_key_down = false;
                break;
            case VK_RIGHT:
                is_right_key_down = false;
                break;
            }
            break;
        }
        break;
    }
}

void Player::move_and_collide(int delta)	{
    float last_velocity_y = velocity.y;

    velocity.y += gravity * (float)delta;
    position += velocity * (float)delta;

    if (hp <= 0)
        return;

    if (velocity.y > 0)
    {
        for (const Platform& platform : platform_list)
        {
            const Platform::CollisionShape& shape = platform.shape;
            bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left))
                <= size.x + (shape.right - shape.left);
            bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

            if (is_collide_x && is_collide_y)
            {
                float delta_pos_y = velocity.y * delta;
                float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
                if (last_tick_foot_pos_y <= shape.y)
                {
                    position.y = shape.y - size.y;
                    velocity.y = 0;

                    if (last_velocity_y !=0)
                    {
                        on_land();
                    }
                    break;
                }
            }
        }
    }

    if (is_invulnerable)
        return;

    for (Bullet* bullet : bullet_list)
    {
        if (!bullet->getValid() || bullet->getCollideTarget() != id)
            continue;

        if (bullet->checkCollision(position, size))
        {
            make_invulnerable();
            bullet->collide();
            hp -= bullet->getDamage();
            last_hurt_direction = bullet->getPosition() - position;

            if (hp <= 0)
            {
                velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
                velocity.y = -1.0f;
            }
        }
    }
    for (Bullet* bullet : effort_bullets)
    {
        if (!bullet->getValid())
            continue;

        if (bullet->checkCollision(position, size))bullet->collide();
    }
}

void Player::set_id(PlayerID id)
{
    this->id = id;
    if(id==PlayerID::P1)statusBar->setPos(235,625);
    else statusBar->setPos(675,625);
}

void Player::on_land()
{
    is_land_effect_visible = true;
    animation_land_effect.reset();

    IMAGE* effect_frame = animation_land_effect.getFrame();
    position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
    position_land_effect.y = position.y + size.y - effect_frame->getheight();
}


void Player::on_run(float dir_x)
{
    if (is_attacking_ex)
        return;

    position.x += dir_x;
    timer_run_effect_generation.resume();
}

void Player::on_jump()
{
    if (velocity.y != 0 || is_attacking_ex)
        return;

    velocity.y += jump_velocity;
    is_jump_effect_visible = true;
    animation_jump_effect.reset();

    IMAGE* effect_frame = animation_jump_effect.getFrame();
    position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
    position_jump_effect.y = position.y + size.y - effect_frame->getheight();
}

void Player::randomSummonEffortBullets()
{
    if (rand() % 100 < 5)
    {
    }
}