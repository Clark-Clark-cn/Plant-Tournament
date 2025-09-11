#include <graphics.h>
#include <mutex>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Msimg32.lib")

#include "head/Atlas.h"
#include "head/base.h"
#include "head/Camera.h"
#include "head/Images.h"
#include "head/screens/game.h"
#include "head/screens/mainmenu.h"
#include "head/screens/manager.h"
#include "head/screens/selector.h"

Player* player_1 = nullptr;
Player* player_2 = nullptr;


std::vector<Bullet*> bullet_list{};
std::vector<Platform> platform_list{};

int FPS = 60;

bool is_debug = false;

Camera camera;

ExMessage msg;

ScreenManager screenManager;
Screen* Menu = nullptr;
Screen* Game = nullptr;
Screen* Selector = nullptr;

void flip_atlas(Atlas& src, Atlas& dest)
{
    dest.clear();
    for (int i = 0; i < src.size(); i++)
    {
        IMAGE img_flipped;
        flip_image(src.getImage(i), &img_flipped);
        dest.addImage(img_flipped);
    }
}

void load_game_resources()
{
    AddFontResourceEx(L"res/IPix.ttf", FR_PRIVATE, 0);
    loadimage(&img_menu_bg, L"res/menu_background.png");
    loadimage(&img_VS, L"res/VS.png");
    loadimage(&img_1P, L"res/1P.png");
    loadimage(&img_2P, L"res/2P.png");
    loadimage(&img_1p_desc, L"res/1P_desc.png");
    loadimage(&img_2p_desc, L"res/2P_desc.png");
    loadimage(&img_gravestone_right, L"res/gravestone.png");
    flip_image(&img_gravestone_right, &img_gravestone_left);
    loadimage(&img_selector_tip, L"res/selector_tip.png");
    loadimage(&img_selector_bg, L"res/selector_background.png");
    loadimage(&img_1p_selector_btn_idle_right, L"res/1P_selector_btn_idle.png");
    flip_image(&img_1p_selector_btn_idle_right, &img_1p_selector_btn_idle_left);
    loadimage(&img_1p_selector_btn_down_right, L"res/1P_selector_btn_down.png");
    flip_image(&img_1p_selector_btn_down_right, &img_1p_selector_btn_down_left);
    loadimage(&img_2p_selector_btn_idle_right, L"res/2P_selector_btn_idle.png");
    flip_image(&img_2p_selector_btn_idle_right, &img_2p_selector_btn_idle_left);
    loadimage(&img_2p_selector_btn_down_right, L"res/2P_selector_btn_down.png");
    flip_image(&img_2p_selector_btn_down_right, &img_2p_selector_btn_down_left);
    loadimage(&img_peashooter_selector_bg_right, L"res/peashooter_selector_background.png");
    flip_image(&img_peashooter_selector_bg_right, &img_peashooter_selector_bg_left);
    loadimage(&img_sunflower_selector_bg_right, L"res/sunflower_selector_background.png");
    flip_image(&img_sunflower_selector_bg_right, &img_sunflower_selector_bg_left);
    loadimage(&img_nut_selector_bg_right, L"res/nut_selector_background.png");
    flip_image(&img_nut_selector_bg_right, &img_nut_selector_bg_left);
    loadimage(&img_gloomshroom_selector_bg_right, L"res/gloomshroom_selector_background.png");
    flip_image(&img_gloomshroom_selector_bg_right, &img_gloomshroom_selector_bg_left);
    img_yellowmshroom_selector_bg_left = img_gloomshroom_selector_bg_left;
    img_yellowmshroom_selector_bg_right = img_gloomshroom_selector_bg_right;

    loadimage(&img_sky, L"res/sky.png");
    loadimage(&img_hills, L"res/hills.png");
    loadimage(&img_platform_large, L"res/platform_large.png");
    loadimage(&img_platform_small, L"res/platform_small.png");

    loadimage(&img_1p_cursor, L"res/1P_cursor.png");
    loadimage(&img_2p_cursor, L"res/2P_cursor.png");

    atlas_peashooter_idle_right.loadimage(L"res/peashooter_idle_%d.png",9);
    flip_atlas(atlas_peashooter_idle_right, atlas_peashooter_idle_left);
    atlas_peashooter_run_right.loadimage(L"res/peashooter_run_%d.png",5);
    flip_atlas(atlas_peashooter_run_right, atlas_peashooter_run_left);
    atlas_peashooter_attack_ex_right.loadimage(L"res/peashooter_attack_ex_%d.png",3);
    flip_atlas(atlas_peashooter_attack_ex_right, atlas_peashooter_attack_ex_left);
    atlas_peashooter_die_right.loadimage(L"res/peashooter_die_%d.png",4);
    flip_atlas(atlas_peashooter_die_right, atlas_peashooter_die_left);

    atlas_sunflower_idle_right.loadimage(L"res/sunflower_idle_%d.png",8);
    flip_atlas(atlas_sunflower_idle_right, atlas_sunflower_idle_left);
    atlas_sunflower_run_right.loadimage(L"res/sunflower_run_%d.png",5);
    flip_atlas(atlas_sunflower_run_right, atlas_sunflower_run_left);
    atlas_sunflower_attack_ex_right.loadimage(L"res/sunflower_attack_ex_%d.png",9);
    flip_atlas(atlas_sunflower_attack_ex_right, atlas_sunflower_attack_ex_left);
    atlas_sunflower_die_right.loadimage(L"res/sunflower_die_%d.png",2);
    flip_atlas(atlas_sunflower_die_right, atlas_sunflower_die_left);

    atlas_nut_idle_right.loadimage(L"res/nut_idle_%d.png",3);
    flip_atlas(atlas_nut_idle_right, atlas_nut_idle_left);
    atlas_nut_run_right.loadimage(L"res/nut_run_%d.png",3);
    flip_atlas(atlas_nut_run_right, atlas_nut_run_left);
    atlas_nut_attack_ex_right.loadimage(L"res/nut_attack_ex_%d.png",5);
    flip_atlas(atlas_nut_attack_ex_right, atlas_nut_attack_ex_left);
    atlas_nut_die_right.loadimage(L"res/nut_die_%d.png",3);
    flip_atlas(atlas_nut_die_right, atlas_nut_die_left);

    atlas_gloomshroom_idle_right.loadimage(L"res/gloomshroom_idle_%d.png",4);
    flip_atlas(atlas_gloomshroom_idle_right, atlas_gloomshroom_idle_left);
    atlas_gloomshroom_run_right.loadimage(L"res/gloomshroom_run_%d.png",3);
    flip_atlas(atlas_gloomshroom_run_right, atlas_gloomshroom_run_left);
    atlas_gloomshroom_attack_ex_right.loadimage(L"res/gloomshroom_attack_ex_%d.png",7);
    flip_atlas(atlas_gloomshroom_attack_ex_right, atlas_gloomshroom_attack_ex_left);
    atlas_gloomshroom_die_right.loadimage(L"res/gloomshroom_die_%d.png",3);
    flip_atlas(atlas_gloomshroom_die_right, atlas_gloomshroom_die_left);

    atlas_yellowmshroom_idle_right.loadimage(L"res/yellowmshroom_idle_%d.png",4);
    flip_atlas(atlas_yellowmshroom_idle_right, atlas_yellowmshroom_idle_left);
    atlas_yellowmshroom_run_right.loadimage(L"res/yellowmshroom_run_%d.png",3);
    flip_atlas(atlas_yellowmshroom_run_right, atlas_yellowmshroom_run_left);
    atlas_yellowmshroom_attack_ex_right.loadimage(L"res/yellowmshroom_attack_ex_%d.png",7);
    flip_atlas(atlas_yellowmshroom_attack_ex_right, atlas_yellowmshroom_attack_ex_left);
    atlas_yellowmshroom_die_right.loadimage(L"res/yellowmshroom_die_%d.png",3);
    flip_atlas(atlas_yellowmshroom_die_right, atlas_yellowmshroom_die_left);

    loadimage(&img_pea, L"res/pea.png");
    loadimage(&img_butter, L"res/butter.png");
    loadimage(&img_cop_cannon, L"res/cop_cannon.png");
    atlas_pea_break.loadimage(L"res/pea_break_%d.png",3);
    atlas_sun.loadimage(L"res/sun_%d.png",5);
    atlas_sun_explode.loadimage(L"res/sun_explode_%d.png",5);
    atlas_sun_ex.loadimage(L"res/sun_ex_%d.png",5);
    atlas_sun_ex_explode.loadimage(L"res/sun_ex_explode_%d.png",5);
    atlas_sun_text.loadimage(L"res/sun_text_%d.png",6);
    atlas_bubble.loadimage(L"res/bubbles_%d.png",7);
    atlas_bubble_ex.loadimage(L"res/bubbles_ex_%d.png",7);
    atlas_nut_explode.loadimage(L"res/nut_explode_%d.png",5);

    atlas_run_effect.loadimage(L"res/run_effect_%d.png",4);
    atlas_land_effect.loadimage(L"res/land_effect_%d.png",2);
    atlas_jump_effect.loadimage(L"res/jump_effect_%d.png",5);

    loadimage(&img_1p_winner, L"res/1P_winner.png");
    loadimage(&img_2p_winner, L"res/2P_winner.png");
    loadimage(&img_winner_bar, L"res/winner_bar.png");

    loadimage(&img_avatar_peashooter, L"res/avatar_peashooter.png");
    loadimage(&img_avatar_sunflower, L"res/avatar_sunflower.png");
    loadimage(&img_avatar_nut, L"res/avatar_nut.png");
    loadimage(&img_avatar_gloomshroom, L"res/avatar_gloomshroom.png");
    loadimage(&img_avatar_yellowmshroom, L"res/avatar_yellowmshroom.png");

    loadimage(&img_butter_splat, L"res/butter_splat.png");
    loadimage(&img_buff_icon_hurry, L"res/buff_icon_hurry.png");
    loadimage(&img_buff_icon_invisible, L"res/buff_icon_invisible.png");
    loadimage(&img_buff_icon_recover, L"res/buff_icon_recover.png");
    loadimage(&img_buff_icon_silence, L"res/buff_icon_silence.png");

    atlas_buff_box_blue.loadimage(L"res/buff_box_blue_%d.png",4);
    atlas_buff_box_pink.loadimage(L"res/buff_box_pink_%d.png",4);
    atlas_buff_box_yellow.loadimage(L"res/buff_box_yellow_%d.png",4);
}
void load_audio_resources()
{
    mciSendString(L"open res/bgm_game.mp3 alias bgm_game", NULL, 0, NULL);
    mciSendString(L"open res/bgm_menu.mp3 alias bgm_menu", NULL, 0, NULL);
    mciSendString(L"open res/pea_break_1.mp3 alias pea_break_1", NULL, 0, NULL);
    mciSendString(L"open res/pea_break_2.mp3 alias pea_break_2", NULL, 0, NULL);
    mciSendString(L"open res/pea_break_3.mp3 alias pea_break_3", NULL, 0, NULL);
    mciSendString(L"open res/pea_shoot_1.mp3 alias pea_shoot_1", NULL, 0, NULL);
    mciSendString(L"open res/pea_shoot_2.mp3 alias pea_shoot_2", NULL, 0, NULL);
    mciSendString(L"open res/pea_shoot_ex.mp3 alias pea_shoot_ex", NULL, 0, NULL);
    mciSendString(L"open res/sun_explode.mp3 alias sun_explode", NULL, 0, NULL);
    mciSendString(L"open res/sun_explode_ex.mp3 alias sun_explode_ex", NULL, 0, NULL);
    mciSendString(L"open res/sun_text.mp3 alias sun_text", NULL, 0, NULL);
    mciSendString(L"open res/bubbles_shot.mp3 alias bubbles_shot", NULL, 0, NULL);
    mciSendString(L"open res/bubbles_shot_ex.mp3 alias bubbles_shot_ex", NULL, 0, NULL);
    mciSendString(L"open res/nut_explode.mp3 alias nut_explode", NULL, 0, NULL);
    mciSendString(L"open res/launch.mp3 alias launch", NULL, 0, NULL);

    mciSendString(L"open res/butter.ogg alias butter", NULL, 0, NULL);
    mciSendString(L"open res/boom.wav alias boom", NULL, 0, NULL);

    mciSendString(L"open res/nut_dash.wav alias nut_dash", NULL, 0, NULL);
    mciSendString(L"open res/ui_confirm.wav alias ui_confirm", NULL, 0, NULL);
    mciSendString(L"open res/ui_switch.wav alias ui_switch", NULL, 0, NULL);
    mciSendString(L"open res/ui_win.wav alias ui_win", NULL, 0, NULL);
}

int main()
{
    load_game_resources();
    Menu = new mainmenu();
    Game = new game();
    Selector = new selector();


#ifdef _DEBUG
    HWND hwnd =  initgraph(1280, 720, EX_SHOWCONSOLE);
#else
    HWND hwnd =  initgraph(1280, 720);
#endif
    SetWindowText(hwnd, L"BeanVSSunflower");
    settextstyle(28,0,L"IPix");
    setbkmode(TRANSPARENT);

    BeginBatchDraw();
    std::mutex updateMutex;
    std::thread([&]{
        load_audio_resources();
        screenManager.setCurrentState(Menu);
        while(true){
            DWORD currentTime = GetTickCount();
            while(peekmessage(&msg))
            {
                screenManager.input(msg);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            std::lock_guard<std::mutex> lock(updateMutex);
            static DWORD lastTime = GetTickCount();
            screenManager.update(currentTime - lastTime);
            lastTime = currentTime;
        }
    }).detach();
    while(true){
        DWORD frameStartTime = GetTickCount();

        cleardevice();
        {
            std::lock_guard<std::mutex> lock(updateMutex);
            screenManager.draw(camera);
        }
        FlushBatchDraw();
        DWORD frameTime = GetTickCount() - frameStartTime;

        if (frameTime < (1000 / FPS))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds((1000 / FPS) - frameTime));
        }
    }

    EndBatchDraw();
    return 0;
}