#pragma once
#include "screen.h"

#include "manager.h"
#include "../base.h"
#include "../platform.h"
#include "../players/Player.h"
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
extern Camera camera;

class game : public Screen
{
    POINT pos_img_sky={0, 0};
    POINT pos_img_hills={0, 0};
    POINT pos_img_winner_bar={0, 0};
    POINT pos_img_winner_text={0, 0};
    IMAGE* img_winner=nullptr;

    int pos_x_img_winner_bar_dst=0;
    int pos_x_img_winner_text_dst=0;
    const float gravity = 5.0f;
    bool is_game_over = false;
    Timer timer_winner_slide_in;
    Timer timer_winner_slide_out;
    bool is_slide_out_started=false;
    int speed_winner_bar=3;
    int speed_winner_text=1.5;
public:
    game(){}
    ~game(){}
    void enter(){
        mciSendString(L"play bgm_game from 0 repeat", nullptr, 0, nullptr);
        is_game_over=false;
        is_slide_out_started=false;
        pos_img_winner_bar={ (-img_winner_bar.getwidth()), (getheight()-img_winner_bar.getheight())/2 };
        pos_img_winner_text={ (pos_img_winner_bar.x), (getheight()-img_1p_winner.getheight())/2 };
        pos_x_img_winner_text_dst=(getwidth()-img_1p_winner.getwidth())/2;
        pos_img_sky = {(getwidth()-img_sky.getwidth())/2, (getheight()-img_sky.getheight())/2};
        pos_img_hills = {(getwidth()-img_hills.getwidth())/2, (getheight()-img_hills.getheight())/2};
        timer_winner_slide_in.restart();
        timer_winner_slide_in.set_wait_time(2500);
        timer_winner_slide_in.set_one_shot(true);
        timer_winner_slide_in.set_callback([&]{
            is_slide_out_started=true;
        });
        timer_winner_slide_out.restart();
        timer_winner_slide_out.set_wait_time(1000);
        timer_winner_slide_out.set_one_shot(true);
        timer_winner_slide_out.set_callback([&]{
            screenManager.switchTo(ScreenManager::ScreenType::MainMenu);
        });
        platform_list.clear();
        platform_list.resize(6);
        Platform& ground = platform_list[0];
        ground.image=&img_platform_large;
        ground.render_pos.x=122;
        ground.render_pos.y=455;
        ground.shape = {(float)ground.render_pos.x+30, 
            (float)ground.render_pos.x+img_platform_large.getwidth()-30,
            (float)ground.render_pos.y+60
        };
        Platform& platform1 = platform_list[1];
        platform1.image = &img_platform_small;
        platform1.render_pos = {200, 300};
        platform1.shape = {
            (float)platform1.render_pos.x+40,
            (float)platform1.render_pos.x + img_platform_small.getwidth()-40,
            (float)platform1.render_pos.y + img_platform_small.getheight()/2
        };
        Platform& platform2 = platform_list[2];
        platform2.image = &img_platform_small;
        platform2.render_pos = {400, 200};
        platform2.shape = {
            (float)platform2.render_pos.x+40,
            (float)platform2.render_pos.x + img_platform_small.getwidth()-40,
            (float)platform2.render_pos.y + img_platform_small.getheight()/2
        };
        Platform& platform3 = platform_list[3];
        platform3.image = &img_platform_small;
        platform3.render_pos = {600, 100};
        platform3.shape = {
            (float)platform3.render_pos.x+40,
            (float)platform3.render_pos.x + img_platform_small.getwidth()-40,
            (float)platform3.render_pos.y + img_platform_small.getheight()/2
        };
        Platform& platform4 = platform_list[4];
        platform4.image = &img_platform_small;
        platform4.render_pos = {800, 200};
        platform4.shape = {
            (float)platform4.render_pos.x+40,
            (float)platform4.render_pos.x + img_platform_small.getwidth()-40,
            (float)platform4.render_pos.y + img_platform_small.getheight()/2
        };
        Platform& platform5 = platform_list[5];
        platform5.image = &img_platform_small;
        platform5.render_pos = {1000, 300};
        platform5.shape = {
            (float)platform5.render_pos.x+40,
            (float)platform5.render_pos.x + img_platform_small.getwidth()-40,
            (float)platform5.render_pos.y + img_platform_small.getheight()/2
        };
        player_1->set_position(200, 50);
        player_2->set_position(975, 50);
    }
    void update(int delta){
        player_1->update(delta);
        player_2->update(delta);
        camera.update(delta);
        bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(),
            [](const Bullet* bullet) { 
                bool deletable = bullet->checkCanRemove();
                if(deletable) delete bullet;
                return deletable;
             }), bullet_list.end());
        for(auto& bullet:bullet_list){
            bullet->update(delta);
        }
        if(player_1->get_hp()<=0||player_2->get_hp()<=0){
            if(!is_game_over){
                is_game_over=true;
                mciSendString(L"stop bgm_game", nullptr, 0, nullptr);
                mciSendString(L"play ui_win from 0", nullptr, 0, nullptr);
                img_winner=player_1->get_hp()>0?&img_1p_winner:&img_2p_winner;
            }
        }
        if(is_game_over){
            pos_img_winner_bar.x+=(int)(speed_winner_bar*delta);
            pos_img_winner_text.x+=(int)(speed_winner_text*delta);
            if(!is_slide_out_started){
                timer_winner_slide_in.update(delta);
                pos_img_winner_bar.x = std::min<LONG>(pos_img_winner_bar.x, pos_x_img_winner_bar_dst);
                pos_img_winner_text.x = std::min<LONG>(pos_img_winner_text.x, pos_x_img_winner_text_dst);
            }
            else timer_winner_slide_out.update(delta);
        }
    }
    void exit(){
        mciSendString(L"stop bgm_game",nullptr,0,nullptr);
        img_winner=nullptr;
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
        putImage(camera,pos_img_sky.x, pos_img_sky.y, &img_sky);
        putImage(camera,pos_img_hills.x, pos_img_hills.y, &img_hills);
        for(auto& platform:platform_list)
            platform.draw(camera);
        player_1->draw(camera);
        player_2->draw(camera);
        for(auto& bullet:bullet_list){
            bullet->draw(camera);
        }
        if(is_game_over){
            putImage(pos_img_winner_bar.x,pos_img_winner_bar.y,&img_winner_bar);
            putImage(pos_img_winner_text.x,pos_img_winner_text.y,img_winner);
        }
    }
    void input(const ExMessage& msg){
        player_1->input(msg);
        player_2->input(msg);
    }
};