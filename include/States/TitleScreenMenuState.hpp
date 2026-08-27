#pragma once

#include "State.hpp"
#include "GUI/Button.hpp"

/**
 * @brief Main title screen menu state.
 *
 * The TitleScreenMenuState class represents the main menu of the game where
 * players can choose to start a new session or open the options menu.
 */
class TitleScreenMenuState : public State {
private:
    /// Main game title text header.
    sf::Text titleText;

    /// Interactive button to launch the character selection or game session.
    GUI::Button playButton;

    /// Interactive button to open the option settings menu.
    GUI::Button optionButton;

private:
    /**
     * @brief Initializes and centers the main title text string.
     */
    void initTitle();

    /**
     * @brief Initializes interactive menu buttons and binds their callback functions.
     */
    void initButtons();

    /**
     * @brief Initializes the background graphics or texture.
     */
    void initBackground();

public:
    /**
     * @brief Constructs a new TitleScreenMenuState instance.
     *
     * @param context Shared global context toolbox.
     */
    TitleScreenMenuState(Context context);

    /**
     * @brief Handles real-time polling inputs per frame.
     */
    void handleInput() override;

    /**
     * @brief Handles incoming SFML window events (mouse clicks, key presses).
     *
     * @param event Constant reference to the sf::Event being processed.
     */
    void handleEvent(const sf::Event& event) override;

    /**
     * @brief Updates title screen logic and button hover animations per frame.
     *
     * @param deltaTime Frame duration time delta measured in seconds.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renders title text and GUI buttons onto the target render window.
     *
     * @param window Target render window.
     */
    void draw(sf::RenderWindow& window) override;
};