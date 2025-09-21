#pragma once

#include "baseItem/Atlas.h"
#include "baseItem/Audio.h"
#include "baseItem/Font.h"

IMAGE img_menu_bg;

IMAGE img_VS;
IMAGE img_1P;
IMAGE img_2P;
IMAGE img_1p_desc;
IMAGE img_2p_desc;
IMAGE img_gravestone;
IMAGE img_selector_tip;
IMAGE img_selector_bg;
IMAGE img_1p_selector_btn_idle;
IMAGE img_1p_selector_btn_down;
IMAGE img_2p_selector_btn_idle;
IMAGE img_2p_selector_btn_down;

IMAGE img_peashooter_selector_bg;
IMAGE img_sunflower_selector_bg;
IMAGE img_nut_selector_bg;
IMAGE img_gloomshroom_selector_bg;
IMAGE img_buttermshroom_selector_bg;

IMAGE img_sky;
IMAGE img_hills;
IMAGE img_platform_large;
IMAGE img_platform_small;

IMAGE img_1p_cursor;
IMAGE img_2p_cursor;

Atlas atlas_peashooter_idle;
Atlas atlas_peashooter_run;
Atlas atlas_peashooter_attack_ex;
Atlas atlas_peashooter_die;

Atlas atlas_sunflower_idle;
Atlas atlas_sunflower_run;
Atlas atlas_sunflower_attack_ex;
Atlas atlas_sunflower_die;

Atlas atlas_nut_idle;
Atlas atlas_nut_run;
Atlas atlas_nut_attack_ex;
Atlas atlas_nut_die;

Atlas atlas_gloomshroom_idle;
Atlas atlas_gloomshroom_run;
Atlas atlas_gloomshroom_attack_ex;
Atlas atlas_gloomshroom_die;

Atlas atlas_buttermshroom_idle;
Atlas atlas_buttermshroom_run;
Atlas atlas_buttermshroom_attack_ex;
Atlas atlas_buttermshroom_die;

IMAGE img_pea;
IMAGE img_butter;
IMAGE img_cop_cannon;
Atlas atlas_pea_break;
Atlas atlas_sun;
Atlas atlas_sun_explode;
Atlas atlas_sun_ex;
Atlas atlas_sun_ex_explode;
Atlas atlas_sun_text;
Atlas atlas_bubble;
Atlas atlas_bubble_ex;
Atlas atlas_nut_explode;

Atlas atlas_run_effect;
Atlas atlas_land_effect;
Atlas atlas_jump_effect;

IMAGE img_1p_winner;
IMAGE img_2p_winner;
IMAGE img_winner_bar;

IMAGE img_avatar_peashooter;
IMAGE img_avatar_sunflower;
IMAGE img_avatar_nut;
IMAGE img_avatar_gloomshroom;
IMAGE img_avatar_buttermshroom;

IMAGE img_butter_splat;
IMAGE img_buff_icon_hurry;
IMAGE img_buff_icon_invisible;
IMAGE img_buff_icon_recover;
IMAGE img_buff_icon_silence;

Atlas atlas_buff_box_blue;
Atlas atlas_buff_box_pink;
Atlas atlas_buff_box_yellow;

IMAGE* img_player1_avatar=nullptr;
IMAGE* img_player2_avatar=nullptr;

Font font_main;

Audio bgm_game;
Audio bgm_menu;
Audio pea_break[3];
Audio pea_shoot[2];
Audio pea_shoot_ex;
Audio sun_explode;
Audio sun_explode_ex;
Audio sun_text;
Audio bubbles_shot;
Audio bubbles_shot_ex;
Audio nut_explode;
Audio launch;

Audio butter;
Audio boom;

Audio nut_dash;
Audio ui_confirm;
Audio ui_switch;
Audio ui_win;