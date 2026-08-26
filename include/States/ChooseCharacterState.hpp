#pragma once
#include "State.hpp"

class ChooseCharacterState : public State {
private:

public:

	ChooseCharacterState(Context context);

	void handleEvent(const sf::Event& event) override;
	void handleInput() override;
	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;
};