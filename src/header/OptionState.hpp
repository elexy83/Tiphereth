#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class Game;

class OptionState : public State {

private:

	Game* game;
	sf::Font font;
	sf::Text textRes;
	sf::Text textFull;
	sf::Text textBack;
	sf::Text textQuite;

public:

	OptionState(Game* game);

	void initGame(Game* game);
	void initFont();
	void initText();

	void handleEvent(sf::Event event) override;
	void handleInput() override;
	void update(float deltaTime) override;
	void draw(sf::RenderWindow& window) override;

	void updateTexts();


};