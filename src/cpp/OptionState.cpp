#include "../header/OptionState.hpp"
#include "../header/Game.hpp"
#include <iostream>


OptionState::OptionState(Game* game)
{
	this->initGame(game);
	this->initFont();
	this->initText();
}

void OptionState::initGame(Game* game)
{
	this->game = game;
}

void OptionState::initFont()
{
	if (!this->font.loadFromFile("assets/fonts/Orbitron/static/Orbitron-Regular.ttf"))
	{
		std::cout << "Font not load in OptionState" << std::endl;
	}
}

void OptionState::initText()
{
	this->textRes.setFont(this->font);
	this->textFull.setFont(this->font);
	this->textBack.setFont(this->font);
	this->textQuite.setFont(this->font);

	this->textRes.setCharacterSize(50);
	this->textFull.setCharacterSize(50);
	this->textBack.setCharacterSize(30);
	this->textQuite.setCharacterSize(30);

	this->textRes.setPosition(1920.f / 4.f, 1080.f / 3.f);
	this->textFull.setPosition(1920.f / 4.f, 1080.f / 2.f);
	this->textBack.setPosition(1920.f / 4.f, 1080.f / 1.5f);
	this->textQuite.setPosition(1920.f / 4.f, 1080.f / 1.f);

	if (this->game->GetIsInGame())
	{
		this->textBack.setString("[ECHAP] Retour au jeu");
	}
	else
	{
		this->textBack.setString("[ECHAP] Retour au menu");
	}

	this->updateTexts();

}

void OptionState::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->game->popState();
		}

		if (event.key.code == sf::Keyboard::L && this->game->GetIsInGame())
		{
			this->game->setIsInGame(false);
			this->game->popState();
		}

		//Toggle FullScreen
		if (event.key.code == sf::Keyboard::F)
		{
			this->game->setFullScreen(!this->game->getIsFullscreen());
			this->game->updateWindow();
			this->updateTexts();

		}

		if (event.key.code == sf::Keyboard::R)
		{
			this->game->nextVideoMode();

			if (!this->game->getIsFullscreen())
			{
				this->game->updateWindow();
			}

			this->updateTexts();
		}

	}
}

void OptionState::handleInput()
{

}

void OptionState::update(float deltaTime)
{

}

void OptionState::draw(sf::RenderWindow& window)
{
	window.draw(this->textRes);
	window.draw(this->textFull);
	window.draw(this->textBack);
	window.draw(this->textQuite);
}

void OptionState::updateTexts()
{
	std::string resString = "[R] Resolution : " + std::to_string(this->game->getResWidth()) + "x" + std::to_string(this->game->getResHeight());
	this->textRes.setString(resString);

	std::string fullString = "[F] Plein écran : ";
	fullString += (this->game->getIsFullscreen()) ? "OUI" : "NON";
	this->textFull.setString(fullString);

	std::string QuiteString = "[L] Quitter : ";
	fullString += (this->game->getIsFullscreen()) ? "OUI" : "NON";
	this->textQuite.setString(QuiteString);
}

