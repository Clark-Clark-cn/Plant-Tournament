#pragma once
#include "screen.h"
#include "manager.h"
#include "players/Player.h"
#include "players/Peashooter.h"
#include "players/Sunflower.h"
#include "players/Gloomshroom.h"
#include "players/Buttermshroom.h"
#include "players/Nut.h"
#include "baseItem/Animation.h"

extern Camera camera;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1p_desc;
extern IMAGE img_2p_desc;
extern IMAGE img_gravestone;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_bg;
extern IMAGE img_1p_selector_btn_idle;
extern IMAGE img_1p_selector_btn_down;
extern IMAGE img_2p_selector_btn_idle;
extern IMAGE img_2p_selector_btn_down;
extern IMAGE img_peashooter_selector_bg;
extern IMAGE img_sunflower_selector_bg;

extern Atlas atlas_peashooter_idle;
extern Atlas atlas_sunflower_idle;

extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;
extern IMAGE img_avatar_gloomshroom;
extern IMAGE img_avatar_buttermshroom;
extern IMAGE img_avatar_nut;

extern ScreenManager screenManager;

extern Player* player_1;
extern Player* player_2;
extern IMAGE* img_player1_avatar;
extern IMAGE* img_player2_avatar;

extern Audio ui_confirm;
extern Audio ui_switch;
extern Audio bgm_menu;

extern Font font_main;

class selector:public Screen
{
    std::vector<std::shared_ptr<Button>> buttons;
public:
	selector()=default;
	~selector()=default;

	void enter()
	{
		static const int OFFSET_X = 50;

		animation_peashooter.setAtlas(&atlas_peashooter_idle);
		animation_peashooter.setInterval(100);
		animation_peashooter.setLoop(true);

		animation_sunflower.setAtlas(&atlas_sunflower_idle);
		animation_sunflower.setInterval(100);
		animation_sunflower.setLoop(true);

		animation_gloomshroom.setAtlas(&atlas_gloomshroom_idle);
		animation_gloomshroom.setInterval(100);
		animation_gloomshroom.setLoop(true);

		animation_nut.setAtlas(&atlas_nut_idle);
		animation_nut.setInterval(100);
		animation_nut.setLoop(true);

		animation_buttermshroom.setAtlas(&atlas_buttermshroom_idle);
		animation_buttermshroom.setInterval(100);
		animation_buttermshroom.setLoop(true);

		pos_img_VS.x = (WINDOW_WIDTH - img_VS.getWidth()) / 2;
		pos_img_VS.y = (WINDOW_HEIGHT - img_VS.getHeight()) / 2;
		pos_img_tip.x = (WINDOW_WIDTH-img_selector_tip.getWidth()) / 2;
		pos_img_tip.y = WINDOW_HEIGHT - 125;
		pos_img_1P.x = WINDOW_WIDTH / 2 - (WINDOW_WIDTH / 2 + img_1P.getWidth()) / 2 - OFFSET_X;
		pos_img_1P.y = 35;
		pos_img_2P.x = WINDOW_WIDTH / 2 + (WINDOW_WIDTH / 2 - img_2P.getWidth()) / 2 + OFFSET_X;
		pos_img_2P.y = pos_img_1P.y;
		pos_img_1P_desc.x = (WINDOW_WIDTH / 2 - img_1p_desc.getWidth())/ 2 - OFFSET_X;
		pos_img_1P_desc.y = WINDOW_HEIGHT - 150;
		pos_img_2P_desc.x = WINDOW_WIDTH / 2 + (WINDOW_WIDTH / 2 - img_2p_desc.getWidth()) / 2 + OFFSET_X;
		pos_img_2P_desc.y = pos_img_1P_desc.y;
		pos_img_1P_gravestone.x = (WINDOW_WIDTH / 2 - img_gravestone.getWidth()) / 2 - OFFSET_X;
		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getHeight() + 35;
		pos_img_2P_gravestone.x = WINDOW_WIDTH / 2 + (WINDOW_WIDTH / 2 - img_gravestone.getWidth()) / 2 + OFFSET_X;
		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
		pos_animation_1P.x = (WINDOW_WIDTH / 2 - atlas_peashooter_idle.getImage(0)->getWidth()) / 2 - OFFSET_X;
		pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
		pos_animation_2P.x = WINDOW_WIDTH / 2 + (WINDOW_WIDTH / 2 - atlas_peashooter_idle.getImage(0)->getWidth()) / 2 + OFFSET_X;
		pos_animation_2P.y = pos_animation_1P.y;
		pos_img_1P_name.y = pos_animation_1P.y + 155;
		pos_img_2P_name.y = pos_img_1P_name.y;
		pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1p_selector_btn_idle.getWidth();
		pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone.getHeight() - img_1p_selector_btn_idle.getHeight()) / 2;
		pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone.getWidth();
		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2p_selector_btn_idle.getWidth();
		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
		pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone.getWidth();
		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
		buttons.resize(5);
		buttons[0] = std::make_shared<Button>();
		buttons[0]->setPosition(pos_1P_selector_btn_left);
		buttons[0]->addItem(&img_1p_selector_btn_idle, SDL_FLIP_HORIZONTAL, pos_1P_selector_btn_left, "idle");
		buttons[0]->addItem(&img_1p_selector_btn_down, SDL_FLIP_HORIZONTAL, pos_1P_selector_btn_left, "down");
		buttons[0]->setActiveRenderActionSet("idle");
		buttons[0]->setCallback(Button::inputResult::Clicked, [this]() {
			buttons[0]->setActiveRenderActionSet("down");
			is_btn_1P_left_down=true;
		});
		buttons[0]->setCallback(Button::inputResult::Released, [this]() {
			buttons[0]->setActiveRenderActionSet("idle");
			is_btn_1P_left_down=false;
			player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
			ui_switch.play();
		});
		buttons[1] = std::make_shared<Button>();
		buttons[1]->setPosition(pos_1P_selector_btn_right);
		buttons[1]->addItem(&img_1p_selector_btn_idle, SDL_FLIP_NONE, pos_1P_selector_btn_right, "idle");
		buttons[1]->addItem(&img_1p_selector_btn_down, SDL_FLIP_NONE, pos_1P_selector_btn_right, "down");
		buttons[1]->setActiveRenderActionSet("idle");
		buttons[1]->setCallback(Button::inputResult::Clicked, [this]() {
			buttons[1]->setActiveRenderActionSet("down");
			is_btn_1P_right_down=true;
		});
		buttons[1]->setCallback(Button::inputResult::Released, [this]() {
			buttons[1]->setActiveRenderActionSet("idle");
			is_btn_1P_right_down=false;
			player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 + 1) % (int)PlayerType::Invalid);
			ui_switch.play();
		});
		buttons[2] = std::make_shared<Button>();
		buttons[2]->setPosition(pos_2P_selector_btn_left);
		buttons[2]->addItem(&img_2p_selector_btn_idle, SDL_FLIP_HORIZONTAL, pos_2P_selector_btn_left, "idle");
		buttons[2]->addItem(&img_2p_selector_btn_down, SDL_FLIP_HORIZONTAL, pos_2P_selector_btn_left, "down");
		buttons[2]->setActiveRenderActionSet("idle");
		buttons[2]->setCallback(Button::inputResult::Clicked, [this]() {
			buttons[2]->setActiveRenderActionSet("down");
			is_btn_2P_left_down=true;
		});
		buttons[2]->setCallback(Button::inputResult::Released, [this]() {
			buttons[2]->setActiveRenderActionSet("idle");
			is_btn_2P_left_down=false;
			player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
			ui_switch.play();
		});
		buttons[3] = std::make_shared<Button>();
		buttons[3]->setPosition(pos_2P_selector_btn_right);
		buttons[3]->addItem(&img_2p_selector_btn_idle, SDL_FLIP_NONE, pos_2P_selector_btn_right, "idle");
		buttons[3]->addItem(&img_2p_selector_btn_down, SDL_FLIP_NONE, pos_2P_selector_btn_right, "down");
		buttons[3]->setActiveRenderActionSet("idle");
		buttons[3]->setCallback(Button::inputResult::Clicked, [this]() {
			buttons[3]->setActiveRenderActionSet("down");
			is_btn_2P_right_down=true;
		});
		buttons[3]->setCallback(Button::inputResult::Released, [this]() {
			buttons[3]->setActiveRenderActionSet("idle");
			is_btn_2P_right_down=false;
			player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 + 1) % (int)PlayerType::Invalid);
			ui_switch.play();
		});
		buttons[4] = std::make_shared<Button>();
		buttons[4]->addItem(&img_selector_tip, SDL_FLIP_NONE, pos_img_tip);
		buttons[4]->setCallback(Button::inputResult::Clicked, [this]() {
			screenManager.switchTo(ScreenManager::ScreenType::Game);
			ui_confirm.play();
		});
	}

	void update(float delta)
	{
		animation_peashooter.update(delta);
		animation_sunflower.update(delta);
		animation_gloomshroom.update(delta);
		animation_nut.update(delta);
		animation_buttermshroom.update(delta);

		selector_bg_scroll_offset_x += 0.5*delta;

		if (selector_bg_scroll_offset_x >= img_peashooter_selector_bg.getWidth())
			selector_bg_scroll_offset_x = 0;
	}

	void draw(const Camera& camera)
	{
		IMAGE* img_p1_selector_bg = nullptr;
		IMAGE* img_p2_selector_bg = nullptr;

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			img_p1_selector_bg = &img_peashooter_selector_bg;
			break;
		case PlayerType::Sunflower:
			img_p1_selector_bg = &img_sunflower_selector_bg;
			break;
		case PlayerType::Gloomshroom:
			img_p1_selector_bg = &img_gloomshroom_selector_bg;
			break;
		case PlayerType::Nut:
			img_p1_selector_bg = &img_nut_selector_bg;
			break;
		case PlayerType::Buttermshroom:
			img_p1_selector_bg = &img_buttermshroom_selector_bg;
			break;
		default:
			img_p1_selector_bg = &img_peashooter_selector_bg;
			break;
		}

		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			img_p2_selector_bg = &img_peashooter_selector_bg;
			break;
		case PlayerType::Sunflower:
			img_p2_selector_bg = &img_sunflower_selector_bg;
			break;
		case PlayerType::Gloomshroom:
			img_p2_selector_bg = &img_gloomshroom_selector_bg;
			break;
		case PlayerType::Nut:
			img_p2_selector_bg = &img_nut_selector_bg;
			break;
		case PlayerType::Buttermshroom:
			img_p2_selector_bg = &img_buttermshroom_selector_bg;
			break;
		default:
			img_p2_selector_bg = &img_peashooter_selector_bg;
			break;
		}

		camera.draw(&img_selector_bg);

		SDL_FRect dst={(float)selector_bg_scroll_offset_x-img_p1_selector_bg->getWidth()
		,0,(float)img_p1_selector_bg->getWidth(),(float)img_p1_selector_bg->getHeight()};
		SDL_Rect src={};
		camera.draw(img_p1_selector_bg,&dst);
		dst={(float)selector_bg_scroll_offset_x,0,
			(float)img_p1_selector_bg->getWidth()-selector_bg_scroll_offset_x,
			(float)img_p1_selector_bg->getHeight()};
		src={0,0,
			(int)(img_p1_selector_bg->getWidth()-selector_bg_scroll_offset_x),
			img_p1_selector_bg->getHeight()};
		camera.draw(img_p1_selector_bg->getTexture(),&src,&dst);
		dst={(float)WINDOW_WIDTH-selector_bg_scroll_offset_x,0,
			(float)img_p1_selector_bg->getWidth(),(float)img_p1_selector_bg->getHeight()};
		camera.draw(img_p2_selector_bg, &dst);
		dst={(float)WINDOW_WIDTH-img_p2_selector_bg->getWidth(),0,
			(float)img_p2_selector_bg->getWidth()-selector_bg_scroll_offset_x,
			(float)img_p2_selector_bg->getHeight()};
		src={(int)selector_bg_scroll_offset_x,0,
			(int)(img_p2_selector_bg->getWidth()-selector_bg_scroll_offset_x),
			img_p2_selector_bg->getHeight()};
		camera.draw(img_p2_selector_bg->getTexture(),&src,&dst);

		camera.draw(pos_img_VS,&img_VS);
		camera.draw(pos_img_1P,&img_1P);
		camera.draw(pos_img_2P,&img_2P);

		camera.draw(pos_img_1P_gravestone,&img_gravestone,SDL_FLIP_HORIZONTAL);
		camera.draw(pos_img_2P_gravestone,&img_gravestone);

		switch (player_type_1)
		{
		case PlayerType::Peashooter:
			camera.draw(pos_animation_1P, animation_peashooter.getFrame(), SDL_FLIP_NONE);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			camera.draw(pos_animation_1P, animation_sunflower.getFrame(), SDL_FLIP_NONE);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
			break;
		case PlayerType::Gloomshroom:
			camera.draw(pos_animation_1P, animation_gloomshroom.getFrame(), SDL_FLIP_NONE);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_gloomshroom_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_gloomshroom_name);
			break;
		case PlayerType::Nut:
			camera.draw(pos_animation_1P, animation_nut.getFrame(), SDL_FLIP_NONE);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_nut_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_nut_name);
			break;
		case PlayerType::Buttermshroom:
			camera.draw(pos_animation_1P, animation_buttermshroom.getFrame(), SDL_FLIP_NONE);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_buttermshroom_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_buttermshroom_name);
			break;
		default:break;
		}

		switch (player_type_2)
		{
		case PlayerType::Peashooter:
			camera.draw(pos_animation_2P, animation_peashooter.getFrame(), SDL_FLIP_HORIZONTAL);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
			break;
		case PlayerType::Sunflower:
			camera.draw(pos_animation_2P, animation_sunflower.getFrame(), SDL_FLIP_HORIZONTAL);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
			break;
		case PlayerType::Gloomshroom:
			camera.draw(pos_animation_2P, animation_gloomshroom.getFrame(), SDL_FLIP_HORIZONTAL);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_gloomshroom_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_gloomshroom_name);
			break;
		case PlayerType::Nut:
			camera.draw(pos_animation_2P, animation_nut.getFrame(), SDL_FLIP_HORIZONTAL);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_nut_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_nut_name);
			break;
		case PlayerType::Buttermshroom:
			camera.draw(pos_animation_2P, animation_buttermshroom.getFrame(), SDL_FLIP_HORIZONTAL);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone.getWidth() - textwidth(str_buttermshroom_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_buttermshroom_name);
			break;
		default:break;
		}

		for(auto& button : buttons)button->draw(camera);
		camera.draw(pos_img_1P_desc, &img_1p_desc);
		camera.draw(pos_img_2P_desc, &img_2p_desc);

		camera.draw(pos_img_tip, &img_selector_tip);
	}

	void input(const SDL_Event& msg)
	{
		switch (msg.type)
		{
		case SDL_KEYDOWN:
			switch (msg.key.keysym.sym)
			{
			case 'a':
				is_btn_1P_left_down = true;
				buttons[0]->setActiveRenderActionSet("down");
				break;
			case 'd':
				is_btn_1P_right_down = true;
				buttons[1]->setActiveRenderActionSet("down");
				break;
			case SDLK_LEFT:
				is_btn_2P_left_down = true;
				buttons[2]->setActiveRenderActionSet("down");
				break;
			case SDLK_RIGHT:
				is_btn_2P_right_down = true;
				buttons[3]->setActiveRenderActionSet("down");
				break;
			}
			break;
		case SDL_KEYUP:
			switch (msg.key.keysym.sym)
			{
			case 'a':
				is_btn_1P_left_down = false;
				buttons[0]->setActiveRenderActionSet("idle");
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 - 1) % (int)PlayerType::Invalid);
				ui_switch.play();
				break;
			case 'd':
				is_btn_1P_right_down = false;
				buttons[1]->setActiveRenderActionSet("idle");
				player_type_1 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_1 + 1) % (int)PlayerType::Invalid);
				ui_switch.play();
				break;
			case SDLK_LEFT:
				is_btn_2P_left_down = false;
				buttons[2]->setActiveRenderActionSet("idle");
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 - 1) % (int)PlayerType::Invalid);
				ui_switch.play();
				break;
			case SDLK_RIGHT:
				is_btn_2P_right_down = false;
				buttons[3]->setActiveRenderActionSet("idle");
				player_type_2 = (PlayerType)(((int)PlayerType::Invalid + (int)player_type_2 + 1) % (int)PlayerType::Invalid);
				ui_switch.play();
				break;
			case SDLK_RETURN:
				screenManager.switchTo(ScreenManager::ScreenType::Game);
				ui_confirm.play();
				break;
			}
			break;
		}
		for(auto& button : buttons)button->input(msg);
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
		case PlayerType::Gloomshroom:
			player_1 = new Gloomshroom();
			img_player1_avatar = &img_avatar_gloomshroom;
			break;
		case PlayerType::Nut:
			player_1 = new Nut();
			img_player1_avatar = &img_avatar_nut;
			break;
		case PlayerType::Buttermshroom:
			player_1 = new Buttermshroom();
			img_player1_avatar = &img_avatar_buttermshroom;
			break;
		default:break;
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
		case PlayerType::Gloomshroom:
			player_2 = new Gloomshroom(false);
			img_player2_avatar = &img_avatar_gloomshroom;
			break;
		case PlayerType::Nut:
			player_2 = new Nut(false);
			img_player2_avatar = &img_avatar_nut;
			break;
		case PlayerType::Buttermshroom:
			player_2 = new Buttermshroom(false);
			img_player2_avatar = &img_avatar_buttermshroom;
			break;
		default:break;
		}
		player_2->set_id(PlayerID::P2);

		bgm_menu.stop();
	}

private:
	void outtextxy_shaded(int x, int y, const std::string& str)
	{
		font_main.drawText(str, { 0, 0, 0, 255 }, { x + 3.0f, y + 3.0f });
		font_main.drawText(str, { 255, 255, 255, 255 }, { (float)x, (float)y });
	}
	int textwidth(const std::string& str)
	{
		return font_main.textwidth(str);
	}

private:
	enum class PlayerType
	{
		Peashooter = 0,
		Sunflower,
		Gloomshroom,
		Nut,
		Buttermshroom,
		Invalid
	};

private:
	bool is_btn_1P_left_down = false;
	bool is_btn_1P_right_down = false;
	bool is_btn_2P_left_down = false;
	bool is_btn_2P_right_down = false;
	Animation animation_peashooter;
	Animation animation_sunflower;
	Animation animation_gloomshroom;
	Animation animation_nut;
	Animation animation_buttermshroom;
	Vector2 pos_img_VS = { 0 };
	Vector2 pos_img_1P = { 0 };
	Vector2 pos_img_tip = { 0 };
	Vector2 pos_img_2P = { 0 };
	Vector2 pos_img_1P_desc = { 0 };
	Vector2 pos_img_1P_name = { 0 };
	Vector2 pos_img_2P_desc = { 0 };
	Vector2 pos_img_2P_name = { 0 };
	Vector2 pos_animation_1P = { 0 };
	Vector2 pos_animation_2P = { 0 };
	Vector2 pos_img_1P_gravestone = { 0 };
	Vector2 pos_img_2P_gravestone = { 0 };
	Vector2 pos_1P_selector_btn_left = { 0 };
	Vector2 pos_1P_selector_btn_right = { 0 };
	Vector2 pos_2P_selector_btn_left = { 0 };
	Vector2 pos_2P_selector_btn_right = { 0 };
	std::string str_peashooter_name = "- beanshooter -";
	std::string str_sunflower_name = "- sunflower -";
	std::string str_gloomshroom_name = "- gloomshroom -";
	std::string str_nut_name = "- nut -";
	std::string str_buttermshroom_name = "- buttermshroom -";
	PlayerType player_type_1 = PlayerType::Peashooter;
	PlayerType player_type_2 = PlayerType::Sunflower;
	float selector_bg_scroll_offset_x = 0;
};