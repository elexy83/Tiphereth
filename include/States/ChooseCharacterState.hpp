#pragma once

#include "State.hpp"

/**
 * @brief Character selection screen state.
 *
 * The ChooseCharacterState class handles the character selection interface
 * where players can pick their gamemode types before starting the game session.
 */
class ChooseCharacterState : public State {
private:
    // Future character selection private variables can be added here.

public:
    /**
     * @brief Constructs a new ChooseCharacterState instance.
     *
     * @param context The shared global context toolbox containing window, game, and asset managers.
     */
    ChooseCharacterState(Context context);

    /**
     * @brief Handles SFML window events (such as key presses or mouse clicks).
     *
     * @param event Constant reference to the sf::Event being processed.
     */
    void handleEvent(const sf::Event& event) override;

    /**
     * @brief Handles real-time polling inputs.
     */
    void handleInput() override;

    /**
     * @brief Updates state logic per frame.
     *
     * @param deltaTime The frame duration time delta measured in seconds.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renders character selection UI elements onto the render window.
     *
     * @reference sf::RenderWindow Target render window.
     */
    void draw(sf::RenderWindow& window) override;
};