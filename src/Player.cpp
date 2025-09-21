#include "players/Player.h"
#include "StatusBar.h"
#include "bullets/efforts.h"
#include "baseItem/Config.h"

extern IMAGE img_1p_cursor;
extern IMAGE img_2p_cursor;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_butter_splat;

extern std::vector<Bullet*> bullet_list;
std::vector<EffortBullet*> effort_bullets;
extern std::vector<Platform> platform_list;

extern bool is_debug;
extern bool is_game_over;


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

    current_animation = &animation_idle;

    timer_invulnerable.setWaitTime(Config::getInstance()->getInt("player.timer.invulnerable"));
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

    timer_cursor_visibility.setWaitTime(Config::getInstance()->getInt("player.timer.cursor_visibility"));
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
            const IMAGE* frame = atlas_run_effect.getImage(0);
            particle_position.x = position.x + (size.x - frame->getWidth()) / 2;
            particle_position.y = position.y + size.y - frame->getHeight();
            particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
        }
    );

    timer_die_effect_generation.setWaitTime(35);
    timer_die_effect_generation.set_callback([&]()
        {
            Vector2 particle_position;
            const IMAGE* frame = atlas_run_effect.getImage(0);
            particle_position.x = position.x + (size.x - frame->getWidth()) / 2;
            particle_position.y = position.y + size.y - frame->getHeight();
            particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
        }
    );

    timer_attack_cd.setOneShot(true);
    timer_attack_cd.set_callback([&]()
        {
            can_attack = true;
        }
    );
    timer_butter.setWaitTime(Config::getInstance()->getInt("player.timer.buttered"));
    timer_butter.setOneShot(true);
    timer_butter.set_callback([&]()
        {
            is_buttered = false;
        }
    );
    timer_silence.setWaitTime(Config::getInstance()->getInt("player.timer.silenced"));
    timer_silence.setOneShot(true);
    timer_silence.set_callback([&]()
        {
            is_silenced = false;
        }
    );
    timer_recover.setWaitTime(Config::getInstance()->getInt("player.timer.recovering"));
    timer_recover.setOneShot(true);
    timer_recover.set_callback([&]()
        {
            is_recovering = false;
        }
    );
    timer_hurry.setWaitTime(Config::getInstance()->getInt("player.timer.hurrying"));
    timer_hurry.setOneShot(true);
    timer_hurry.set_callback([&]()
        {
            is_hurrying = false;
        }
    );
    timer_invisible.setWaitTime(Config::getInstance()->getInt("player.timer.invisible"));
    timer_invisible.setOneShot(true);
    timer_invisible.set_callback([&]()
        {
            is_invisible = false;
        }
    );
}

void Player::update(float delta)
{
    int direction = is_right_key_down - is_left_key_down;
    if(is_facing_right)current_animation->setFlipMode(SDL_FLIP_NONE);
    else current_animation->setFlipMode(SDL_FLIP_HORIZONTAL);

    if (direction != 0)
    {
        if (!is_attacking_ex)
        {
            is_facing_right = direction > 0;
            current_animation = &animation_run;
            int delta_run=0;
            if(!is_hurrying)delta_run=direction * run_velocity * delta;
            else delta_run=direction * run_velocity * delta * hurry_multiplier;
            on_run(delta_run);
        }
    }
    else
    {
        current_animation = &animation_idle;
        timer_run_effect_generation.pause();
    }

    if (is_attacking_ex)
    {
        current_animation = &animation_attack_ex;
    }

    if (hp <= 0)
    {
        current_animation = &animation_die;
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
        current_animation->getFrame()->sketch(&img_sketch);
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

    if(position.y>WINDOW_HEIGHT)hp=0;

    if(is_recovering&&hp<max_hp)hp+=recover_multiplier*delta;

    if(is_silenced)timer_silence.update(delta);
    if(is_recovering)timer_recover.update(delta);
    if(is_hurrying)timer_hurry.update(delta);
    if(is_invisible)timer_invisible.update(delta);
    if(is_buttered)timer_butter.update(delta);
    if(is_game_over)statusBar->setVisible(false);
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

    if (hp > 0 && is_invulnerable && is_showing_sketch_frame && !is_invisible)
    {
        camera.draw(position,&img_sketch);
    }
    else if(!is_invisible)
    {
        current_animation->draw(camera, (int)position.x, (int)position.y);
    }
    statusBar->draw();

    if(is_buttered){
        camera.draw(position.x+size.x/4,position.y,&img_butter_splat);
    }

    if (is_debug)
    {
        camera.setColor({0,125,255});
        SDL_Rect rect = {(int)position.x,(int)position.y,(int)size.x,(int)size.y};
        camera.rect(rect);
    }

    if (is_cursor_visible){
        switch (id)
        {
        case PlayerID::P1:
            camera.draw(position.x + (size.x - img_1p_cursor.getWidth()) / 2, position.y - img_1p_cursor.getHeight(), &img_1p_cursor);
            break;
        case PlayerID::P2:
            camera.draw(position.x + (size.x - img_2p_cursor.getWidth()) / 2, position.y - img_2p_cursor.getHeight(), &img_2p_cursor);
            break;
        }
    }
}

void Player::input(const SDL_Event& msg){
    if (hp <= 0)
        return;
    if(is_buttered)
        return;
    static const int p1_left_key=Config::getInstance()->getInt("player.key.p1.left");
    static const int p1_right_key=Config::getInstance()->getInt("player.key.p1.right");
    static const int p1_jump_key=Config::getInstance()->getInt("player.key.p1.jump");
    static const int p1_attack_key=Config::getInstance()->getInt("player.key.p1.attack");
    static const int p1_attackEx_key=Config::getInstance()->getInt("player.key.p1.attackEx");
    static const int p2_left_key=Config::getInstance()->getInt("player.key.p2.left");
    static const int p2_right_key=Config::getInstance()->getInt("player.key.p2.right");
    static const int p2_jump_key=Config::getInstance()->getInt("player.key.p2.jump");
    static const int p2_attack_key=Config::getInstance()->getInt("player.key.p2.attack");
    static const int p2_attackEx_key=Config::getInstance()->getInt("player.key.p2.attackEx");
    switch (msg.type)
    {
    case SDL_KEYDOWN:
        switch (id)
        {
        case PlayerID::P1:
            if(msg.key.keysym.sym==p1_left_key)is_left_key_down = true;
            else if(msg.key.keysym.sym==p1_right_key)is_right_key_down = true;
            else if(msg.key.keysym.sym==p1_jump_key)on_jump();
            else if(msg.key.keysym.sym==p1_attack_key){
                if (can_attack&&!is_silenced)
                {
                    attack();
                    can_attack = false;
                    timer_attack_cd.restart();
                }
            }
            else if(msg.key.keysym.sym==p1_attackEx_key){
                if (mp >= 100)
                {
                    attackEx();
                    mp = 0;
                }
            }
            break;
        case PlayerID::P2:
            if(msg.key.keysym.sym==p2_left_key)is_left_key_down = true;
            else if(msg.key.keysym.sym==p2_right_key)is_right_key_down = true;
            else if(msg.key.keysym.sym==p2_jump_key)on_jump();
            else if(msg.key.keysym.sym==p2_attack_key){
                if (can_attack&&!is_silenced)
                {
                    attack();
                    can_attack = false;
                    timer_attack_cd.restart();
                }
            }
            else if(msg.key.keysym.sym==p2_attackEx_key){
                if (mp >= 100)
                {
                    attackEx();
                    mp = 0;
                }
            }
            break;
        }
        break;
    case SDL_KEYUP:
        switch (id)
        {
        case PlayerID::P1:
            if(msg.key.keysym.sym==p1_left_key)is_left_key_down = false;
            else if(msg.key.keysym.sym==p1_right_key)is_right_key_down = false;
            break;
        case PlayerID::P2:
            if(msg.key.keysym.sym==p2_left_key)is_left_key_down = false;
            else if(msg.key.keysym.sym==p2_right_key)is_right_key_down = false;
            break;
        }
        break;
    }
}

void Player::move_and_collide(int delta)	{
    float last_velocity_y = velocity.y;
    Vector2 old_position = position;

    velocity.y += gravity * (float)delta;
    Vector2 new_position = position + velocity * (float)delta;

    if (hp <= 0){
        position=new_position;
        return;
    }

    if (velocity.y > 0)
    {
        float min_collision_time = 1.0f;
        const Platform* collision_platform =  nullptr;
        for (const Platform& platform : platform_list)
        {
            const Platform::CollisionShape& shape = platform.shape;

            bool is_collide_x = (std::max(new_position.x+size.x,shape.right)-
                std::min(new_position.x,shape.left)<= size.x+(shape.right-shape.left));
            if(!is_collide_x)continue;

            float old_foot_y=old_position.y+size.y;
            float new_foot_y = new_position.y+size.y;

            if(old_foot_y<=shape.y&&new_foot_y>shape.y){
                float collision_time= (shape.y-old_foot_y)/(new_foot_y-old_foot_y);
                if(collision_time>=0.0f&&collision_time<min_collision_time){
                    min_collision_time=collision_time;
                    collision_platform=&platform;
                }
            }
        }
        if(collision_platform){
            position.x=old_position.x+velocity.x*min_collision_time;
            position.y=collision_platform->shape.y - size.y;
            velocity.y=0;
            if(last_velocity_y!=0)on_land();
        }
        else position=new_position;
    }
    else position=new_position;

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
    for (EffortBullet* bullet : effort_bullets)
    {
        if (!bullet->getValid())
            continue;

        if (bullet->checkCollision(position, size))bullet->collide(this);
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

    const IMAGE* effect_frame = animation_land_effect.getFrame();
    position_land_effect.x = position.x + (size.x - effect_frame->getWidth()) / 2;
    position_land_effect.y = position.y + size.y - effect_frame->getHeight();
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

    const IMAGE* effect_frame = animation_jump_effect.getFrame();
    position_jump_effect.x = position.x + (size.x - effect_frame->getWidth()) / 2;
    position_jump_effect.y = position.y + size.y - effect_frame->getHeight();
}