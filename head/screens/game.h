#pragma once
#include "screen.h"

#include "manager.h"
#include "baseItem/base.h"
#include "platform.h"
#include "players/Player.h"
#include "bullets/efforts.h"
#include <algorithm>
#include <vector>

extern ScreenManager screenManager;

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;
extern IMAGE img_1p_winner;
extern IMAGE img_2p_winner;
extern IMAGE img_winner_bar;

extern std::vector<Platform> platform_list;

extern Player* player_1;
extern Player* player_2;

extern std::vector<Bullet*> bullet_list;
extern std::vector<EffortBullet*> effort_bullets;
extern Camera camera;

extern Audio bgm_game;
extern Audio ui_win;

bool is_game_over=false;

class game : public Screen
{
    Vector2 pos_img_sky={0, 0};
    Vector2 pos_img_hills={0, 0};
    Vector2 pos_img_winner_bar={0, 0};
    Vector2 pos_img_winner_text={0, 0};
    IMAGE* img_winner_text=nullptr;

    int pos_x_img_winner_bar_dst=0;
    int pos_x_img_winner_text_dst=0;
    const float gravity = 5.0f;
    Timer timer_winner_slide_in;
    Timer timer_winner_slide_out;
    bool is_slide_out_started=false;
    int speed_winner_bar=3;
    int speed_winner_text=2;
    Timer timer_summon_effort_bullet;
public:
    game(){
        timer_summon_effort_bullet.setWaitTime(10);
        timer_summon_effort_bullet.setOneShot(false);
        timer_summon_effort_bullet.set_callback([&]{
            randomSummonEffortBullets();
        });
    }
    ~game(){}
    void enter(){
        bgm_game.play(-1);
        is_game_over=false;
        is_slide_out_started=false;
        pos_img_winner_bar={ float(-img_winner_bar.getWidth()), (WINDOW_HEIGHT-img_winner_bar.getHeight())/2.0f };
        pos_img_winner_text={ float(pos_img_winner_bar.x), (WINDOW_HEIGHT-img_1p_winner.getHeight())/2.0f };
        pos_x_img_winner_text_dst=(WINDOW_WIDTH-img_1p_winner.getWidth())/2;
        pos_img_sky = {(WINDOW_WIDTH-img_sky.getWidth())/2.0f, (WINDOW_HEIGHT-img_sky.getHeight())/2.0f};
        pos_img_hills = {(WINDOW_WIDTH-img_hills.getWidth())/2.0f, (WINDOW_HEIGHT-img_hills.getHeight())/2.0f};
        timer_winner_slide_in.restart();
        timer_winner_slide_in.setWaitTime(2500);
        timer_winner_slide_in.setOneShot(true);
        timer_winner_slide_in.set_callback([&]{
            is_slide_out_started=true;
        });
        timer_winner_slide_out.restart();
        timer_winner_slide_out.setWaitTime(1000);
        timer_winner_slide_out.setOneShot(true);
        timer_winner_slide_out.set_callback([&]{
            screenManager.switchTo(ScreenManager::ScreenType::MainMenu);
        });
        platform_list.clear();
        platform_list=Config::getInstance()->getPlatforms();
        player_1->set_position(200, 50);
        player_2->set_position(975, 50);
        timer_summon_effort_bullet.restart();
    }
    void update(float delta){
        player_1->update(delta);
        player_2->update(delta);
        camera.update(delta);
        timer_summon_effort_bullet.update(delta);
        bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
            [](const Bullet* bullet) { 
                bool deletable = bullet->checkCanRemove();
                if(deletable) delete bullet;
                return deletable;
             }), bullet_list.end());
        effort_bullets.erase(std::remove_if(effort_bullets.begin(), effort_bullets.end(),
            [](const EffortBullet* bullet) {
                bool deletable = bullet->checkCanRemove();
                if (deletable) delete bullet;
                return deletable;
            }), effort_bullets.end());
        for(auto& bullet:bullet_list){
            bullet->update(delta);
        }
        for(auto& bullet:effort_bullets){
            bullet->update(delta);
        }
        if(player_1->get_hp()<=0||player_2->get_hp()<=0){
            if(!is_game_over){
                is_game_over=true;
                bgm_game.stop();
                ui_win.play();
                img_winner_text=player_1->get_hp()>0?&img_1p_winner:&img_2p_winner;
            }
        }
        if(is_game_over){
            pos_img_winner_bar.x+=(int)(speed_winner_bar*delta);
            pos_img_winner_text.x+=(int)(speed_winner_text*delta);
            if(!is_slide_out_started){
                timer_winner_slide_in.update(delta);
                pos_img_winner_bar.x = std::min<long>(pos_img_winner_bar.x, pos_x_img_winner_bar_dst);
                pos_img_winner_text.x = std::min<long>(pos_img_winner_text.x, pos_x_img_winner_text_dst);
            }
            else timer_winner_slide_out.update(delta);
        }
    }
    void exit(){
        bgm_game.stop();
        img_winner_text=nullptr;
        is_game_over=false;
        is_slide_out_started=false;
        delete player_1;
        delete player_2;
        player_1=nullptr;
        player_2=nullptr;
        bullet_list.clear();
        camera.reset();
    }
    void draw(const Camera& camera){
        camera.draw(&img_sky);
        camera.draw(&img_hills);
        for(auto& platform:platform_list)
            platform.draw(camera);
        player_1->draw(camera);
        player_2->draw(camera);
        for(auto& bullet:bullet_list){
            bullet->draw(camera);
        }
        for(auto& bullet:effort_bullets){
            bullet->draw(camera);
        }
        if(is_game_over){
            camera.draw(pos_img_winner_bar, &img_winner_bar);
            camera.draw(pos_img_winner_text, img_winner_text);
        }
    }
    void input(const SDL_Event& msg){
        player_1->input(msg);
        player_2->input(msg);
    }
    
    void randomSummonEffortBullets()
    {
        if (rand() % Config::getInstance()->getInt("player.multiplier.effort") == 0)
        {
            effort_bullets.push_back(new EffortBullet());
        }
    }
};