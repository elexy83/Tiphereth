#pragma once

#include <SFML/Graphics.hpp>
#include "Core/Context.hpp"

/**
 * @brief Abstract base class representing a generic game state.
 *
 * The State class serves as the interface for all application states (e.g.,
 * main menu, gameplay session, pause menu, options). It stores a shared
 * Context toolbox reference and defines pure virtual hooks for event handling,
 * input processing, frame updates, and rendering.
 */
class State {
protected:
    /// Shared contextual toolkit holding window, game engine, and resource manager pointers.
    Context context;

public:
    /**
     * @brief Constructs a new State instance.
     *
     * @param context The shared Context toolbox structure.
     */
    State(Context context) : context(context) {}

    /**
     * @brief Virtual destructor ensuring proper polymorphic cleanup for derived states.
     */
    virtual ~State() = default;

    /**
     * @brief Handles incoming SFML window events (such as key presses or mouse clicks).
     *
     * @param event Constant reference to the sf::Event being processed.
     */
    virtual void handleEvent(const sf::Event& event) = 0;

    /**
     * @brief Handles real-time polling inputs per frame.
     */
    virtual void handleInput() = 0;

    /**
     * @brief Updates logical state computations per frame.
     *
     * @param deltaTime The frame duration time delta measured in seconds.
     */
    virtual void update(float deltaTime) = 0;

    /**
     * @brief Renders visual components belonging to this state onto the target window.
     *
     * @param window Target SFML render window.
     */
    virtual void draw(sf::RenderWindow& window) = 0;
};