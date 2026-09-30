#pragma once

#include "States/State.hpp"
#include "GUI/Button.hpp"
#include "GUI/Background.hpp"
#include <SFML/Graphics.hpp>

/**
 * @brief Shop of the selected character (placeholder: only the "Play" button for now).
 *
 * "Play" (bottom left) starts the gameplay. Escape goes back to the character selection.
 */
class ShopState : public State {
private:
    enum class PendingAction { None, Play, Back };

    GUI::Background background;
    sf::Text titleText;
    GUI::Button playButton;

    PendingAction pendingAction = PendingAction::None;

private:
    void initUI();

public:
    ShopState(Context context);

    void handleEvent(const sf::Event& event) override;
    void handleInput() override;
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};