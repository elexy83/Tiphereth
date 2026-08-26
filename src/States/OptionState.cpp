#include "States/OptionState.hpp"
#include "Core/Game.hpp"
#include <iostream>


OptionState::OptionState(Context context) : State(context)
{
	this->initFont();
	this->initText();
}

void OptionState::initFont()
{
	this->context.fonts->load(Fonts::ID::Title, "assets/fonts/Orbitron/static/Orbitron-Regular.ttf");
}

void OptionState::initText()
{
	this->textRes.setFont(this->context.fonts->get(Fonts::ID::Title));
	this->textFull.setFont(this->context.fonts->get(Fonts::ID::Title));
	this->textBack.setFont(this->context.fonts->get(Fonts::ID::Title));
	this->textQuite.setFont(this->context.fonts->get(Fonts::ID::Title));

	this->textRes.setCharacterSize(50);
	this->textFull.setCharacterSize(50);
	this->textBack.setCharacterSize(30);
	this->textQuite.setCharacterSize(30);

	this->textRes.setPosition(1920.f / 4.f, 1080.f / 3.f);
	this->textFull.setPosition(1920.f / 4.f, 1080.f / 2.f);
	this->textBack.setPosition(1920.f / 4.f, 1080.f / 1.5f);
	this->textQuite.setPosition(1920.f / 4.f, 1080.f / 1.f);

	if (this->context.game->GetIsInGame())
	{
		this->textBack.setString("[ECHAP] Retour au jeu");
	}
	else
	{
		this->textBack.setString("[ECHAP] Retour au menu");
	}

	this->updateTexts();

}

// Do the input only one time.
void OptionState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->context.game->popState();
		}

		if (event.key.code == sf::Keyboard::L && this->context.game->GetIsInGame())
		{
			this->context.game->setIsInGame(false);
			this->context.game->popState();
		}

		//Toggle FullScreen
		if (event.key.code == sf::Keyboard::F)
		{
			this->context.game->setFullScreen(!this->context.game->getIsFullscreen());
			this->context.game->updateWindow();
			this->updateTexts();
		}

		if (event.key.code == sf::Keyboard::R)
		{
			this->context.game->nextVideoMode();

			if (!this->context.game->getIsFullscreen())
			{
				this->context.game->updateWindow();
			}

			this->updateTexts();
		}

	}
}

void OptionState::handleInput()
{
}


//Do the input or whatever at each frames.
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
	std::string resString = "[R] Resolution : " + std::to_string(this->context.game->getResWidth()) + "x" + std::to_string(this->context.game->getResHeight());
	this->textRes.setString(resString);

	std::string fullString = "[F] Plein écran : ";
	fullString += (this->context.game->getIsFullscreen()) ? "OUI" : "NON";
	this->textFull.setString(fullString);

	std::string QuiteString = "[L] Quitter : ";
	fullString += (this->context.game->getIsFullscreen()) ? "OUI" : "NON";
	this->textQuite.setString(QuiteString);
}

