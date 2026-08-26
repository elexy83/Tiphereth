#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class Game;

class OptionState : public State {

private:
	sf::Font font;
	sf::Text textRes;
	sf::Text textFull;
	sf::Text textBack;
	sf::Text textQuite;

public:

	OptionState(Context context);

	void initFont();
	void initText();

	void handleEvent(const sf::Event& event) override;
	void handleInput() override;
	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

	void updateTexts();


};