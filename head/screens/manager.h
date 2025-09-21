#pragma once
#include "screen.h"
#include "baseItem/MessageBox.h"

extern Screen* Menu;
extern Screen* Game;
extern Screen* Selector;

extern bool is_debug;

class ScreenManager
{
public:
    enum class ScreenType {
        MainMenu,
        Game,
        Selector
    };
    ScreenManager()=default;
    ~ScreenManager()=default;
    void setCurrentState(Screen* newScreen)
    {
        currentScreen = newScreen;
        currentScreen->enter();
    }
    void switchTo(ScreenType newScreenType)
    {
        currentScreen->exit();
        switch (newScreenType)
        {
        case ScreenType::MainMenu:
            currentScreen = Menu;
            break;
        case ScreenType::Game:
            currentScreen = Game;
            break;
        case ScreenType::Selector:
            currentScreen = Selector;
            break;
        }
        currentScreen->enter();
    }
    void input(const SDL_Event& msg)
    {
        switch (msg.type)
        {
        case SDL_KEYDOWN:
            if (msg.key.keysym.sym == SDLK_ESCAPE)
            {
                exit(0);
            }
            else if (msg.key.keysym.sym == SDLK_F1)
            {
                is_debug = !is_debug;
            }
            break;
        case SDL_QUIT:
            exit(0);
        }
        if (currentScreen)
        {
            currentScreen->input(msg);
        }
        for(auto& box:boxes)box->input(msg);
    }
    void update(float delta)
    {
        if (currentScreen)
        {
            currentScreen->update(delta);
        }
    }
    void draw(const Camera& camera)
    {
        if (currentScreen)
        {
            currentScreen->draw(camera);
        }
        for(auto& box : boxes)box->draw();
    }
private:
    Screen* currentScreen=nullptr;
};
