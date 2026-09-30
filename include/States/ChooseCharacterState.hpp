#pragma once

#include "States/State.hpp"
#include "GUI/Button.hpp"
#include "GUI/Background.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>
#include <memory>

/**
 * @brief Character selection screen: one button per character (4 for now).
 *
 * Clicking a character stores the choice in Game and opens the ShopState on top of this state.
 * Escape goes back to the title screen.
 */
class ChooseCharacterState : public State {
public:
    static constexpr std::size_t CharacterCount = 4;

private:
    enum class PendingAction { None, OpenShop, Back };

    GUI::Background background;
    sf::Text titleText;
    std::array<std::unique_ptr<GUI::Button>, CharacterCount> characterButtons;

    /// Action requested by a button, executed at the start of update() (never inside handleEvent).
    PendingAction pendingAction = PendingAction::None;
    int pendingCharacter = -1;

private:
    void initUI();

public:
    ChooseCharacterState(Context context);

    void handleEvent(const sf::Event& event) override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};