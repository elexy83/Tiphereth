#pragma once
#include <SFML/Graphics.hpp>

class State {
public:

    virtual ~State() = default;

    // Works for inputs/events that need to be call only one time and not each frame
    virtual void handleEvent(sf::Event event) = 0;

    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};