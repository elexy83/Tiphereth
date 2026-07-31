#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "States/State.hpp"
#include "Core/Context.hpp"
#include <vector>

class State;
class Game {
private:
    sf::RenderWindow window;
    std::stack<std::unique_ptr<State>> states;
    sf::View view;
    float deltaTime;
    const float VIEW_WIDTH = 1920.f;
    const float VIEW_HEIGHT = 1080.f;
    bool isFullscreen = false;
    unsigned int resWidth = 1280;
    unsigned resHeight = 720;
    std::vector<sf::VideoMode> videoModes;
    int currentVideoModeIndex;
    bool isInGame = false;

    Context context;

public:
    Game();

    //Init methodes
    void initWindow();
    void initView();
    void initState();
    void initContext();

    void run();

    void pushState(std::unique_ptr<State> state);
    void changeState(std::unique_ptr<State> state);
    void updateWindow();
    void popState();

    const unsigned int getResWidth();
    const unsigned int getResHeight();
    const bool getIsFullscreen();
    const int getCurrentVideoModeIndex();
    const std::vector<sf::VideoMode> GetVideoModes();
    const bool GetIsInGame();

    void setFullScreen(bool isFullscreen);
    void setResWidth(unsigned int width);
    void setResHeight(unsigned int height);
    void setCurrentVideoModeIndex(int index);
    void setIsInGame(bool isInGame);

    void nextVideoMode();
};