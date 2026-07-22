#include "../header/TitleScreenMenu.hpp"
#include "../header/Game.hpp"
#include "../header/OptionState.hpp"
#include <iostream>

TitleScreenMenu::TitleScreenMenu(Game* game)
{
	this->initGame(game);
	this->initFont();
	this->initTitle();
	this->initStartText();
	this->initOptionText();
	this->initBackground();

}

void TitleScreenMenu::initGame(Game* game)
{
	this->game = game;
}

void TitleScreenMenu::initFont()
{
	if (!this->titleFont.loadFromFile("assets/fonts/Orbitron/static/Orbitron-Regular.ttf"))
	{
		std::cout << "Font not load in TitleScreenMenu" << std::endl;
	}
}

void TitleScreenMenu::initTitle()
{
	this->titleText.setFont(this->titleFont);
	this->titleText.setString("Tiphereth");
	this->titleText.setCharacterSize(100);
	this->titleText.setFillColor(sf::Color::White);

	// Center text
	sf::FloatRect textRect = this->titleText.getLocalBounds();
	this->titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	this->titleText.setPosition(1920.f / 2.0f, 1080.f / 3.0f);
}

void TitleScreenMenu::initStartText()
{
	this->startText.setFont(this->titleFont);
	this->startText.setString("[Entrer] Jouer");
	this->startText.setCharacterSize(50);
	sf::FloatRect textRect = this->startText.getLocalBounds();
	this->startText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->startText.setPosition(1920.f / 2.0f, 1080.f / 2.0f);
}

void TitleScreenMenu::initOptionText()
{
	this->optionText.setFont(this->titleFont);
	this->optionText.setString("[O] Option");
	this->optionText.setCharacterSize(30);
	sf::FloatRect textRect = this->optionText.getLocalBounds();
	this->optionText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	this->optionText.setPosition(1920.f / 2.0f, 1080.f / 1.5f);
}

void TitleScreenMenu::initBackground()
{
	/* if (!this->backgroundTexture.loadFromFile("assets/textures/menu_bg.png"))
	{
		std::cout << "ERREUR: Impossible de charger menu_bg.png" << std::endl;
	}
	this->backgroundSprite.setTexture(this->backgroundTexture);
	*/
}

void TitleScreenMenu::handleInput()
{

}

void TitleScreenMenu::handleEvent(sf::Event event)
{
	// Check if the event (so only one time) is a key pressed
	if (event.type == sf::Event::KeyPressed)
	{
		// Checking which key is pressed

		// Escape -> close game
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->game->popState();
		}

		// Enter -> start game
		if (event.key.code == sf::Keyboard::Enter)
		{
			//this->game->pushState(std::make_unique<GameState>(this->game));
		}

		if (event.key.code == sf::Keyboard::O)
		{
			this->game->pushState(std::make_unique<OptionState>(this->game));
		}

	}
}

void TitleScreenMenu::update(float deltaTime)
{

}

void TitleScreenMenu::draw(sf::RenderWindow& window)
{
	// window.draw(this->backgroundSprite);
	window.draw(this->titleText);
	window.draw(this->startText);
	window.draw(this->optionText);
}
