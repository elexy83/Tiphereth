#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include "States/State.hpp"
#include "Core/Context.hpp"
#include "States/StateIdentifiers.hpp"
#include <vector>

class State;

/**
 * @brief The core engine and main game loop controller class.
 *
 * The Game class manages the application window, the rendering view, the global
 * resource repositories (textures, fonts), the stack-based game states, and
 * window display configurations (resolutions, full-screen modes).
 */
class Game {
private:
    /// The primary SFML application render window.
    sf::RenderWindow window;

    /// The stack container holding active application states (menus, gameplay, pause).
    std::stack<std::unique_ptr<State>> states;

    /// The base 2D camera view handling scaling and letterboxing.
    sf::View view;

    /// The frame duration time delta measured in seconds.
    float deltaTime;

    /// The fixed logical width for consistent view scaling (1920.0f).
    const float VIEW_WIDTH = 1920.f;

    /// The fixed logical height for consistent view scaling (1080.f).
    const float VIEW_HEIGHT = 1080.f;

    /// Flag indicating whether the window is currently in fullscreen mode.
    bool isFullscreen = false;

    /// Current window width dimension in pixels.
    unsigned int resWidth = 1280;

    /// Current window height dimension in pixels.
    unsigned int resHeight = 720;

    /// List of available full-screen video modes supported by the display device.
    std::vector<sf::VideoMode> videoModes;

    /// Index pointing to the currently active video mode within the videoModes vector.
    int currentVideoModeIndex;

    /// Flag indicating whether an active gameplay session is currently running.
    bool isInGame = false;

    /// Central texture repository preventing redundant disk loads.
    TextureManager textures;

    /// Central font repository preventing redundant disk loads.
    FontManager fonts;

    /// Shared contextual toolkit passed down to all active states.
    Context context;

private:
    /**
     * @brief Factory method responsible for instantiating states dynamically based on their ID.
     *
     * @param stateID The States::ID enumeration key identifying the target state.
     * @return std::unique_ptr<State> A unique pointer to the newly created State instance.
     */
    std::unique_ptr<State> createState(States::ID stateID);

    /**
     * @brief Initializes and creates the application render window based on current video settings.
     */
    void initWindow();

    /**
     * @brief Initializes the fixed logical 2D camera view.
     */
    void initView();

    /**
     * @brief Pushes the initial starting state onto the state stack.
     */
    void initState();

    /**
     * @brief Populates and links the shared context container with window and manager references.
     */
    void initContext();

public:
    /**
     * @brief Constructs a new Game engine instance and initializes core subsystems.
     */
    Game();

    /**
     * @brief Executes the primary core game loop handling events, updates, and rendering.
     */
    void run();

    /**
     * @brief Pushes a new state onto the stack by its identifier, pausing the previous state.
     *
     * @param stateID The States::ID key of the state to push.
     */
    void pushState(States::ID stateID);

    /**
     * @brief Replaces the current top state on the stack with a new state.
     *
     * @param stateID The States::ID key of the state to switch to.
     */
    void changeState(States::ID stateID);

    /**
     * @brief Recreates or updates the render window dimensions and style properties.
     */
    void updateWindow();

    /**
     * @brief Removes the current top state from the stack, returning to the previous one.
     */
    void popState();

    /**
     * @brief Gets the current window resolution width.
     * @return const unsigned int Width in pixels.
     */
    const unsigned int getResWidth();

    /**
     * @brief Gets the current window resolution height.
     * @return const unsigned int Height in pixels.
     */
    const unsigned int getResHeight();

    /**
     * @brief Checks if the game window is currently in fullscreen mode.
     * @return true If fullscreen is enabled.
     * @return false Otherwise.
     */
    const bool getIsFullscreen();

    /**
     * @brief Gets the index of the active video mode.
     * @return const int Video mode index.
     */
    const int getCurrentVideoModeIndex();

    /**
     * @brief Retrieves the list of available full-screen video modes.
     * @return const std::vector<sf::VideoMode> Vector of sf::VideoMode.
     */
    const std::vector<sf::VideoMode> GetVideoModes();

    /**
     * @brief Checks if an active game session is ongoing.
     * @return true If inside a game session.
     * @return false Otherwise.
     */
    const bool GetIsInGame();

    /**
     * @brief Sets the fullscreen state toggle flag.
     * @param isFullscreen True for fullscreen, false for windowed.
     */
    void setFullScreen(bool isFullscreen);

    /**
     * @brief Sets the target window resolution width.
     * @param width Width in pixels.
     */
    void setResWidth(unsigned int width);

    /**
     * @brief Sets the target window resolution height.
     * @param height Height in pixels.
     */
    void setResHeight(unsigned int height);

    /**
     * @brief Sets the active video mode index.
     * @param index Vector index.
     */
    void setCurrentVideoModeIndex(int index);

    /**
     * @brief Sets the in-game session flag status.
     * @param isInGame Boolean value.
     */
    void setIsInGame(bool isInGame);

    /**
     * @brief Cycles to the next available video mode configuration.
     */
    void nextVideoMode();
};