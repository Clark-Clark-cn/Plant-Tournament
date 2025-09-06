#pragma once
#include "screen.h"

#include "manager.h"
#include <iostream>

extern ScreenManager screenManager;

extern IMAGE img_menu_bg;

class mainmenu : public Screen
{
public:
    mainmenu()=default;
    ~mainmenu()=default;
    void enter(){
        mciSendString(L"play bgm_menu repeat from 0", nullptr, 0, nullptr);
    }
    void draw(const Camera& camera) {
        putimage(0,0,&img_menu_bg);
    }
    void input(const ExMessage& msg) {
        if(msg.message==WM_KEYUP){
            mciSendString(L"play ui_confirm from 0", nullptr, 0, nullptr);
            screenManager.switchTo(ScreenManager::ScreenType::Selector);
        }
    }
};