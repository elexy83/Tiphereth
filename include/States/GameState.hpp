#pragma once
#pragma once

#include "States/State.hpp"
#include <SFML/Graphics.hpp>


class GameState : public State {
private:
    sf::Text infoText;
    sf::Text hintText;
    bool requestPop = false;

public:
    GameState(Context context);

    void handleEvent(const sf::Event& event) override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};