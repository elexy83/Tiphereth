#pragma once

#include "States/State.hpp"
#include "GUI/Button.hpp"
#include "Managers/InputManager.hpp"
#include "GUI/Background.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Option menu with two tabs: Video (resolution, FPS, fullscreen, language)
 *        and Controls (per-gameplay key rebinding).
 *
 * Every change is buffered: nothing is kept until "Apply" is pressed.
 * "Back" / Escape discards pending video changes and restores the key bindings.
 */
class OptionState : public State {

private:
    enum class Tab { Video, Controls };

    /// Number of binding rows displayed at once (the list scrolls beyond that).
    static constexpr std::size_t MaxVisibleRows = 7;

    /// One line of the controls list: action name + one button per slot.
    struct BindingRow {
        sf::Text label;
        std::array<std::unique_ptr<GUI::Button>, InputManager::SlotCount> slotButtons;
        bool visible = false;
    };

    // ---- Common ----
    sf::RectangleShape overlay;
    sf::RectangleShape panel;
    sf::Text titleText;
    GUI::Button tabVideoButton;
    GUI::Button tabControlsButton;
    GUI::Button backButton;
    GUI::Button applyButton;
    GUI::Button resetButton;

    // ---- Video tab ----
    sf::Text resLabel;
    sf::Text resValueText;
    sf::Text fpsLabel;
    sf::Text fpsValueText;
    sf::Text fsLabel;
    sf::Text langLabel;
    sf::Text langValueText;
    GUI::Button resPrevButton;
    GUI::Button resNextButton;
    GUI::Button fpsPrevButton;
    GUI::Button fpsNextButton;
    GUI::Button fsToggleBtn;
    GUI::Button langPrevButton;
    GUI::Button langNextButton;

    // ---- Controls tab ----
    sf::Text groupNameText;
    sf::Text headerAction;
    sf::Text headerKey1;
    sf::Text headerKey2;
    sf::Text scrollText;
    sf::Text hintText;
    GUI::Button groupPrevButton;
    GUI::Button groupNextButton;
    std::vector<BindingRow> rows;

    // ---- State ----
    Tab currentTab = Tab::Video;

    int pendingVideoModeIndex = 0;
    int pendingLanguageIndex = 0;
    int pendingFpsIndex = 1;
    bool pendingFullscreen = false;

    std::size_t currentGroup = 0;
    std::size_t scrollOffset = 0;
    int listeningAction = -1;   ///< Absolute action index waiting for a key, -1 = none.
    int listeningSlot = -1;

    /// Snapshot of the bindings, restored when leaving without applying.
    InputManager savedBindings;

    bool requestPop = false;

    /// Set when a left click was used as a binding: its release must not re-trigger a button.
    bool ignoreNextLeftRelease = false;

    std::vector<std::string> availableLanguages = { "fr", "en" };
    std::vector<unsigned int> availableFPS = { 30, 60, 120, 144, 240, 0 }; // 0 = unlimited

    GUI::Background background;

private:
    void initUI();
    void updateTexts();
    void refreshVideoTexts();
    void refreshControlsTexts();
    void refreshTabStyles();

    void setTab(Tab tab);
    void applySettings();
    void requestClose();

    // Key rebinding
    bool isListening() const { return listeningAction >= 0; }
    void beginListening(std::size_t actionIndex, int slot);
    void stopListening();
    void handleBindingCapture(const sf::Event& event);
    void assignBinding(const InputManager::Binding& binding);
    void scrollRows(int delta);
    sf::String bindingToText(const InputManager::Binding& binding) const;

public:
    OptionState(Context context);

    void handleEvent(const sf::Event& event) override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};