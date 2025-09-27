#include <mutex>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Msimg32.lib")

#include "baseItem/Atlas.h"
#include "baseItem/base.h"
#include "baseItem/Camera.h"
#include "res.h"
#include "screens/game.h"
#include "screens/mainmenu.h"
#include "screens/manager.h"
#include "screens/selector.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif
#include <chrono>
#ifndef __EMSCRIPTEN__
    #include <thread>
#endif

Player* player_1 = nullptr;
Player* player_2 = nullptr;

std::vector<Bullet*> bullet_list{};
std::vector<Platform> platform_list{};
std::vector<std::shared_ptr<MessageBox>> boxes;

const int WINDOW_WIDTH=1280;
const int WINDOW_HEIGHT=720;

int FPS = Config::getInstance()->getInt("window.fps");

bool is_running = true;
bool is_debug = false;

Camera camera;
SDL_Renderer* renderer = nullptr;

ScreenManager screenManager;
Screen* Menu = nullptr;
Screen* Game = nullptr;
Screen* Selector = nullptr;
SDL_Window* window = nullptr;

void load_game_resources(SDL_Renderer* renderer)
{
    font_main.load(renderer, "res/font.ttf", 24);
    img_menu_bg.load(renderer,"res/menu_background.png");
    img_VS.load(renderer,"res/VS.png");
    img_1P.load(renderer,"res/1P.png");
    img_2P.load(renderer,"res/2P.png");
    img_1p_desc.load(renderer,"res/1P_desc.png");
    img_2p_desc.load(renderer,"res/2P_desc.png");
    img_gravestone.load(renderer,"res/gravestone.png");
    img_selector_tip.load(renderer,"res/selector_tip.png");
    img_selector_bg.load(renderer,"res/selector_background.png");
    img_1p_selector_btn_idle.load(renderer,"res/1P_selector_btn_idle.png");
    img_1p_selector_btn_down.load(renderer,"res/1P_selector_btn_down.png");
    img_2p_selector_btn_idle.load(renderer,"res/2P_selector_btn_idle.png");
    img_2p_selector_btn_down.load(renderer,"res/2P_selector_btn_down.png");
    img_peashooter_selector_bg.load(renderer,"res/peashooter_selector_background.png");
    img_sunflower_selector_bg.load(renderer,"res/sunflower_selector_background.png");
    img_nut_selector_bg.load(renderer,"res/nut_selector_background.png");
    img_gloomshroom_selector_bg.load(renderer,"res/gloomshroom_selector_background.png");
    img_buttermshroom_selector_bg = img_gloomshroom_selector_bg;

    img_sky.load(renderer,"res/sky.png");
    img_hills.load(renderer,"res/hills.png");
    img_platform_large.load(renderer,"res/platform_large.png");
    img_platform_small.load(renderer,"res/platform_small.png");

    img_1p_cursor.load(renderer,"res/1P_cursor.png");
    img_2p_cursor.load(renderer,"res/2P_cursor.png");

    atlas_peashooter_idle.loadimage(renderer,"res/peashooter_idle_%d.png",9);
    atlas_peashooter_run.loadimage(renderer,"res/peashooter_run_%d.png",5);
    atlas_peashooter_attack_ex.loadimage(renderer,"res/peashooter_attack_ex_%d.png",3);
    atlas_peashooter_die.loadimage(renderer,"res/peashooter_die_%d.png",4);

    atlas_sunflower_idle.loadimage(renderer,"res/sunflower_idle_%d.png",8);
    atlas_sunflower_run.loadimage(renderer,"res/sunflower_run_%d.png",5);
    atlas_sunflower_attack_ex.loadimage(renderer,"res/sunflower_attack_ex_%d.png",9);
    atlas_sunflower_die.loadimage(renderer,"res/sunflower_die_%d.png",2);

    atlas_nut_idle.loadimage(renderer,"res/nut_idle_%d.png",3);
    atlas_nut_run.loadimage(renderer,"res/nut_run_%d.png",3);
    atlas_nut_attack_ex.loadimage(renderer,"res/nut_attack_ex_%d.png",5);
    atlas_nut_die.loadimage(renderer,"res/nut_die_%d.png",3);

    atlas_gloomshroom_idle.loadimage(renderer,"res/gloomshroom_idle_%d.png",4);
    atlas_gloomshroom_run.loadimage(renderer,"res/gloomshroom_run_%d.png",3);
    atlas_gloomshroom_attack_ex.loadimage(renderer,"res/gloomshroom_attack_ex_%d.png",7);
    atlas_gloomshroom_die.loadimage(renderer,"res/gloomshroom_die_%d.png",3);

    atlas_buttermshroom_idle.loadimage(renderer,"res/buttermshroom_idle_%d.png",4);
    atlas_buttermshroom_run.loadimage(renderer,"res/buttermshroom_run_%d.png",3);
    atlas_buttermshroom_attack_ex.loadimage(renderer,"res/buttermshroom_attack_ex_%d.png",7);
    atlas_buttermshroom_die.loadimage(renderer,"res/buttermshroom_die_%d.png",3);

    img_pea.load(renderer,"res/pea.png");
    img_butter.load(renderer,"res/butter.png");
    img_cop_cannon.load(renderer,"res/cop_cannon.png");
    atlas_pea_break.loadimage(renderer,"res/pea_break_%d.png",3);
    atlas_sun.loadimage(renderer,"res/sun_%d.png",5);
    atlas_sun_explode.loadimage(renderer,"res/sun_explode_%d.png",5);
    atlas_sun_ex.loadimage(renderer,"res/sun_ex_%d.png",5);
    atlas_sun_ex_explode.loadimage(renderer,"res/sun_ex_explode_%d.png",5);
    atlas_sun_text.loadimage(renderer,"res/sun_text_%d.png",6);
    atlas_bubble.loadimage(renderer,"res/bubbles_%d.png",7);
    atlas_bubble_ex.loadimage(renderer,"res/bubbles_ex_%d.png",7);
    atlas_nut_explode.loadimage(renderer,"res/nut_explode_%d.png",5);

    atlas_run_effect.loadimage(renderer,"res/run_effect_%d.png",4);
    atlas_land_effect.loadimage(renderer,"res/land_effect_%d.png",2);
    atlas_jump_effect.loadimage(renderer,"res/jump_effect_%d.png",5);

    img_1p_winner.load(renderer,"res/1P_winner.png");
    img_2p_winner.load(renderer,"res/2P_winner.png");
    img_winner_bar.load(renderer,"res/winner_bar.png");

    img_avatar_peashooter.load(renderer,"res/avatar_peashooter.png");
    img_avatar_sunflower.load(renderer,"res/avatar_sunflower.png");
    img_avatar_nut.load(renderer,"res/avatar_nut.png");
    img_avatar_gloomshroom.load(renderer,"res/avatar_gloomshroom.png");
    img_avatar_buttermshroom.load(renderer,"res/avatar_buttermshroom.png");

    img_butter_splat.load(renderer,"res/butter_splat.png");
    img_buff_icon_hurry.load(renderer,"res/buff_icon_hurry.png");
    img_buff_icon_invisible.load(renderer,"res/buff_icon_invisible.png");
    img_buff_icon_recover.load(renderer,"res/buff_icon_recover.png");
    img_buff_icon_silence.load(renderer,"res/buff_icon_silence.png");

    atlas_buff_box_blue.loadimage(renderer,"res/buff_box_blue_%d.png",4);
    atlas_buff_box_pink.loadimage(renderer,"res/buff_box_pink_%d.png",4);
    atlas_buff_box_yellow.loadimage(renderer,"res/buff_box_yellow_%d.png",4);
}
void load_audio_resources()
{
    bgm_game.load("res/bgm_game.mp3");
    bgm_menu.load("res/bgm_menu.mp3");
    pea_break[0].load("res/pea_break_1.mp3");
    pea_break[1].load("res/pea_break_2.mp3");
    pea_break[2].load("res/pea_break_3.mp3");
    pea_shoot[0].load("res/pea_shoot_1.mp3");
    pea_shoot[1].load("res/pea_shoot_2.mp3");
    pea_shoot_ex.load("res/pea_shoot_ex.mp3");
    sun_explode.load("res/sun_explode.mp3");
    sun_explode_ex.load("res/sun_explode_ex.mp3");
    sun_text.load("res/sun_text.mp3");
    bubbles_shot.load("res/bubbles_shot.mp3");
    bubbles_shot_ex.load("res/bubbles_shot_ex.mp3");
    nut_explode.load("res/nut_explode.mp3");
    launch.load("res/launch.mp3");

    butter.load("res/butter.ogg");
    boom.load("res/boom.wav");

    nut_dash.load("res/nut_dash.wav");
    ui_confirm.load("res/ui_confirm.wav");
    ui_switch.load("res/ui_switch.wav");
    ui_win.load("res/ui_win.wav");
}

void mainloop(){
    static auto frameStart = std::chrono::steady_clock::now();
    static auto frameEnd = std::chrono::steady_clock::now();
    static SDL_Event msg;
    while(SDL_PollEvent(&msg))
    {
        screenManager.input(msg);
    }
    std::chrono::nanoseconds delta = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()-frameEnd);
    screenManager.update(delta.count()/1000000.0f);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    screenManager.draw(camera);
    static float FPS_real = 0;
    if(is_debug)font_main.drawText("FPS: "+std::to_string(FPS_real), { 150, 150, 150, 255 }, { 2.0f, (float)(0) });
    SDL_RenderPresent(renderer);
    frameEnd = std::chrono::steady_clock::now();
    int frameTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count();
#ifndef __EMSCRIPTEN__
    if (frameTime < (1000000000 / FPS))
    {
        SDL_Delay((1000000000 / FPS - frameTime) / 1000000);
    }
#endif
    if(frameTime>0)FPS_real=(1000000000.0f/frameTime);
    frameStart=frameEnd;
#ifdef __EMSCRIPTEN__
    if(!is_running)emscripten_cancel_main_loop();
#endif
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
    Mix_Init(MIX_INIT_MP3|MIX_INIT_OGG);
    TTF_Init();

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, Config::getInstance()->getString("window.scale_quality").c_str());

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("PlantTournament", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        1280, 720, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#ifndef __EMSCRIPTEN__
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_RenderSetIntegerScale(renderer, SDL_FALSE);
#endif
    
    camera.setRenderer(renderer);
    load_game_resources(renderer);
    load_audio_resources();
    Menu = new mainmenu();
    Game = new game();
    Selector = new selector();

    screenManager.setCurrentState(Menu);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, 1);
#else
    while(is_running){
        mainloop();
    }
#endif

    return 0;
}