#pragma once
#include "State.hpp"
#include "Game.hpp"


class Game;

class TitleScreenMenu : public State {
private:
    Game* game;

    sf::Text titleText;
    sf::Font titleFont;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Text startText;

    sf::Text optionText;

public:
    // Constructor
    TitleScreenMenu(Game* game);

    // Init
    void initGame(Game* game);
    void initFont();
    void initTitle();
    void initStartText();
    void initOptionText();
    void initBackground();


    void handleInput() override;
    void handleEvent(sf::Event event) override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};