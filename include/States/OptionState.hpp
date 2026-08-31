#pragma once

#include "States/State.hpp"
#include "GUI/Button.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Option menu state allowing players to modify display settings.
 *
 * The OptionState class manages resolution scaling, fullscreen toggles,
 * and configuration adjustments with a pending changes buffer applied upon request.
 */
class OptionState : public State {

private:
    /// Main title text header displayed at the top of the menu.
    sf::Text titleText;

    /// Static label text for the resolution setting row.
    sf::Text resLabel;

    /// Static label text for the fullscreen setting row.
    sf::Text fsLabel;

    /// Dynamic text displaying the currently selected resolution value.
    sf::Text resValueText;

    // Static label text for the language setting row.
    sf::Text langLabel;

    /// Dynamic text displaying the currently selected language value.
    sf::Text langValueText;

    /// Interactive button to apply pending configuration modifications.
    GUI::Button applyButton;

    /// Interactive button to return to the previous state.
    GUI::Button backButton;

    /// Interactive button to terminate the application session.
    GUI::Button quitButton;

    /// Decrement button to cycle backward through available video resolutions.
    GUI::Button resPrevButton;

    /// Increment button to cycle forward through available video resolutions.
    GUI::Button resNextButton;

    /// Decrement button to cycle backward through available languages.
    GUI::Button langPrevButton;

    /// Increment button to cycle forward through available languages.
    GUI::Button langNextButton;

    /// Toggle button to switch between windowed and fullscreen display modes.
    GUI::Button fsToggleBtn;

    /// Buffer tracking the selected video mode index before confirmation.
    int pendingVideoModeIndex;

    /// Buffer tracking the selected language index before confirmation.
    int pendingLanguageIndex;

    /// Buffer tracking the fullscreen toggle state before confirmation.
    bool pendingFullscreen;

    /// Safe-exit security flag requesting state removal on the next frame update.
    bool requestPop;

    /// Safe-exit security flag requesting application closure on the next frame update.
    bool requestQuit;

    std::vector<std::string> availableLanguages = { "fr", "en" };

    sf::Text fpsLabel;

    sf::Text fpsValueText;

    GUI::Button fpsPrevButton;

    GUI::Button fpsNextButton;

    std::vector<unsigned int> availableFPS = { 30, 60, 120, 144, 240, 0 }; // 0 = unlimitted

    int pendingFpsIndex;

private:
    /**
     * @brief Initializes and positions all user interface labels and buttons.
     */
    void initUI();

    /**
     * @brief Refreshes dynamic text strings and centering coordinates.
     */
    void updateTexts();

    /**
     * @brief Applies pending video mode and fullscreen settings to the engine.
     */
    void applySettings();

public:
    /**
     * @brief Constructs a new OptionState instance.
     *
     * @param context Shared global context toolbox.
     */
    OptionState(Context context);

    /**
     * @brief Handles incoming SFML window events (mouse clicks, key presses).
     *
     * @param event Constant reference to the sf::Event being processed.
     */
    void handleEvent(const sf::Event& event) override;

    /**
     * @brief Handles real-time polling inputs.
     */
    void handleInput() override;

    /**
     * @brief Updates state logic per frame and evaluates safe-exit flags.
     *
     * @param deltaTime Frame duration time delta measured in seconds.
     */
    void update(float deltaTime) override;

    /**
     * @brief Renders the option screen GUI components onto the target window.
     *
     * @param window Target render window.
     */
    void draw(sf::RenderWindow& window) override;
};