#pragma once
#include <SFML/Graphics.hpp>
#include "Core/Context.hpp"

class State {
protected:
       Context context;
public:

    State(Context context) : context(context) {}
    virtual ~State() = default;

    // Works for inputs/events that need to be call only one time and not each frame
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void handleInput() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};