#pragma once
#include "screen.h"
#include "manager.h"
#include "../players/Player.h"
#include "../players/Peashooter.h"
#include "../players/Sunflower.h"
#include "../Animation.h"
#include <graphics.h>


extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1p_desc;
extern IMAGE img_2p_desc;
extern IMAGE img_gravestone_left;
extern IMAGE img_gravestone_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_bg;
extern IMAGE img_1p_selector_btn_idle_left;
extern IMAGE img_1p_selector_btn_idle_right;
extern IMAGE img_1p_selector_btn_down_left;
extern IMAGE img_1p_selector_btn_down_right;
extern IMAGE img_2p_selector_btn_idle_left;
extern IMAGE img_2p_selector_btn_idle_right;
extern IMAGE img_2p_selector_btn_down_left;
extern IMAGE img_2p_selector_btn_down_right;
extern IMAGE img_peashooter_selector_bg_left;
extern IMAGE img_peashooter_selector_bg_right;
extern IMAGE img_sunflower_selector_bg_left;
extern IMAGE img_sunflower_selector_bg_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_sunflower_idle_right;

extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;

extern ScreenManager screenManager;

extern Player* player_1;
extern Player* player_2;
extern IMAGE* img_player1_avatar;
extern IMAGE* img_player2_avatar;
class selector:public Screen
{
public:
	selector()=default;
	~selector()=default;

	void enter()
	{
		static const int OFFSET_X = 50;

		animation_peashooter.setAtlas(&atlas_peashooter_idle_right);
		animation_peashooter.setInterval(100);
		animation_peashooter.setLoop(true);

		animation_sunflower.setAtlas(&atlas_sunflower_idle_right);
		animation_sunflower.setInterval(100);
		animation_sunflower.setLoop(true);

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
		pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
		pos_img_tip.x = (getwidth()-img_selector_tip.getwidth()) / 2;
		pos_img_tip.y = getheight() - 125;
		pos_img_1P.x = getwidth() / 2 - (getwidth() / 2 + img_1P.getwidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 35;
		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;
		pos_img_1P_desc.x = (getwidth() / 2 - img_1p_desc.getwidth())/ 2 - OFFSET_X;
		pos_img_1P_desc.y = getheight() - 150;
		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2p_desc.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;
		pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;
		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
		pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;
		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
		pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.getImage(0)->getwidth()) / 2 - OFFSET_X;
		pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
		pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.getImage(0)->getwidth()) / 2 + OFFSET_X;
		pos_animation_2P.y = pos_animation_1P.y;
		pos_img_1P_name.y = pos_animation_1P.y + 155;
		pos_img_2P_name.y = pos_img_1P_name.y;
		pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1p_selector_btn_idle_left.getwidth();
		pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1p_selector_btn_idle_left.getheight()) / 2;
		pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();
		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2p_selector_btn_idle_left.getwidth();
		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();
		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
	}

	void update(int delta)
	{
		animation_peashooter.update(delta);
		animation_sunflower.update(delta);

		selector_bg_scroll_offset_x += 5;

		if (selector_bg_scroll_offset_x >= img_peashooter_selector_bg_left.getwidth())
			selector_bg_scroll_offset_x = 0;
	}

	void draw(const Camera& camera)
	{
		IMAGE* img_p1_selector_bg = nullptr;
		IMAGE* img_p2_selector_bg = nullptr;

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			img_p1_selector_bg = &img_peashooter_selector_bg_right;
			break;
		case PlayerType::Sunflower:
			img_p1_selector_bg = &img_sunflower_selector_bg_right;
			break;
		default:
			img_p1_selector_bg = &img_peashooter_selector_bg_right;
			break;
		}

		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			img_p2_selector_bg = &img_peashooter_selector_bg_left;
			break;
		case PlayerType::Sunflower:
			img_p2_selector_bg = &img_sunflower_selector_bg_left;
			break;
		default:
			img_p2_selector_bg = &img_peashooter_selector_bg_left;
			break;
		}

		putImage(0, 0, &img_selector_bg);
		putImage(selector_bg_scroll_offset_x-img_p1_selector_bg->getwidth(),0,img_p1_selector_bg);
		putImage(selector_bg_scroll_offset_x,0,img_p1_selector_bg->getwidth()-selector_bg_scroll_offset_x,0,
			img_p1_selector_bg,0,0);
		putImage(getwidth()-img_p2_selector_bg->getwidth(),0,img_p2_selector_bg->getwidth()-selector_bg_scroll_offset_x,0,
			img_p2_selector_bg,selector_bg_scroll_offset_x,0);
		putImage(getwidth()-selector_bg_scroll_offset_x,0,img_p2_selector_bg);

		putImage(pos_img_VS.x,pos_img_VS.y,&img_VS);

		putImage(pos_img_1P.x,pos_img_1P.y,&img_1P);
		putImage(pos_img_2P.x,pos_img_2P.y,&img_2P);
		putImage(pos_img_1P_gravestone.x,pos_img_1P_gravestone.y,&img_gravestone_right);
		putImage(pos_img_2P_gravestone.x,pos_img_2P_gravestone.y,&img_gravestone_left);

		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			animation_peashooter.draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			animation_sunflower.draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
			break;
		}

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			animation_peashooter.draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			animation_sunflower.draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
			break;
		}

		putImage(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y, is_btn_1P_left_down ? 
			&img_1p_selector_btn_down_left : &img_1p_selector_btn_idle_left);
		putImage(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y, is_btn_1P_right_down ?
			 &img_1p_selector_btn_down_right : &img_1p_selector_btn_idle_right);
		putImage(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y, is_btn_2P_left_down ?
			 &img_2p_selector_btn_down_left : &img_2p_selector_btn_idle_left);
		putImage(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y, is_btn_2P_right_down ?
			 &img_2p_selector_btn_down_right : &img_2p_selector_btn_idle_right);
		putImage(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1p_desc);
		putImage(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2p_desc);

		putImage(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
	}

	void input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case 'A':
				is_btn_1P_left_down = true;
				break;
			case 'D':
				is_btn_1P_right_down = true;
				break;
			case VK_LEFT:
				is_btn_2P_left_down = true;
				break;
			case VK_RIGHT:
				is_btn_2P_right_down = true;
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 'A':
				is_btn_1P_left_down = false;
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case 'D':
				is_btn_1P_right_down = false;
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 + 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_LEFT:
				is_btn_2P_left_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_RIGHT:
				is_btn_2P_right_down = false;
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 + 1) % (int)PlayerType::Invalid);
				mciSendString(L"play ui_switch from 0", NULL, 0, NULL);
				break;
			case VK_RETURN:
				screenManager.switchTo(ScreenManager::ScreenType::Game);
				mciSendString(L"play ui_confirm from 0", NULL, 0, NULL);
				break;
			}
			break;
		}
	}

	void exit()
	{
		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			player_1 = new Peashooter();
			img_player1_avatar = &img_avatar_peashooter;
			break;
		case PlayerType::Sunflower:
			player_1 = new Sunflower();
			img_player1_avatar = &img_avatar_sunflower;
			break;
		}
		player_1->set_id(PlayerID::P1);

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			player_2 = new Peashooter(false);
			img_player2_avatar = &img_avatar_peashooter;
			break;
		case PlayerType::Sunflower:
			player_2 = new Sunflower(false);
			img_player2_avatar = &img_avatar_sunflower;
			break;
		}
		player_2->set_id(PlayerID::P2);

		mciSendString(L"stop bgm_menu", NULL, 0, NULL);
	}

private:
	void outtextxy_shaded(int x, int y, LPCTSTR str)
	{
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}

private:
	enum class PlayerType
	{
		Peashooter = 0,
		Sunflower,
		Invalid
	};

private:
	bool is_btn_1P_left_down = false;
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;
	Animation animation_peashooter;
	Animation animation_sunflower;
	POINT pos_img_VS = { 0 };
	POINT pos_img_1P = { 0 };
	POINT pos_img_tip = { 0 };
	POINT pos_img_2P = { 0 };
	POINT pos_img_1P_desc = { 0 };
	POINT pos_img_1P_name = { 0 };
	POINT pos_img_2P_desc = { 0 };
	POINT pos_img_2P_name = { 0 };
	POINT pos_animation_1P = { 0 };
	POINT pos_animation_2P = { 0 };
	POINT pos_img_1P_gravestone = { 0 };
	POINT pos_img_2P_gravestone = { 0 };
	POINT pos_1P_selector_btn_left = { 0 };
	POINT pos_1P_selector_btn_right = { 0 };
	POINT pos_2P_selector_btn_left = { 0 };
	POINT pos_2P_selector_btn_right = { 0 };
	LPCTSTR str_peashooter_name = L"beanshooter";
	LPCTSTR str_sunflower_name = L"sunflower";
	PlayerType player_type_1 = PlayerType::Peashooter;
	PlayerType player_type_2 = PlayerType::Sunflower;
	int selector_bg_scroll_offset_x = 0;

};