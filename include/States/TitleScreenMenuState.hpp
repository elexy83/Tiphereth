#pragma once
#include "State.hpp"

class TitleScreenMenuState : public State {
private:

    sf::Text titleText;
    sf::Font titleFont;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Text startText;

    sf::Text optionText;

public:
    // Constructor
    TitleScreenMenuState();

    // Init

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