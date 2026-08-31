#include "States/TitleScreenMenuState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <iostream>

TitleScreenMenuState::TitleScreenMenuState(Context context) 
	: State(context)
	, playButton(context.fonts->get(Fonts::ID::Title))
	, optionButton(context.fonts->get(Fonts::ID::Title))
{
	this->initTitle();
	this->initButtons();
	this->initBackground();
}

void TitleScreenMenuState::initTitle()
{
	this->titleText.setFont(this->context.fonts->get(Fonts::ID::Title));
	this->titleText.setString("Tiphereth");
	this->titleText.setCharacterSize(100);
	this->titleText.setFillColor(sf::Color::White);

	// Center text
	sf::FloatRect textRect = this->titleText.getLocalBounds();
	this->titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	this->titleText.setPosition(1920.f / 2.0f, 1080.f / 3.0f);
}

void TitleScreenMenuState::initButtons()
{
	// Play button
	playButton.setText(this->context.i18n->get("menu.play"));
	playButton.setPosition(1920.f / 2.0f, 1080.f / 2.0f);

	playButton.setCallback([this]()
	{
		this->context.game->changeState(States::ID::ChooseCharacter);
	});

	// Option button
	optionButton.setText(this->context.i18n->get("menu.options"));
	optionButton.setPosition(1920.f / 2.0f, 1080.f / 1.5f);

	optionButton.setCallback([this]()
	{
		// Using pushState instead of changeState to be able to get back to the TitleScreenMenu
		this->context.game->pushState(States::ID::Option);
	});
}

void TitleScreenMenuState::initBackground()
{

}


void TitleScreenMenuState::handleInput()
{
}

void TitleScreenMenuState::handleEvent(const sf::Event& event)
{
	playButton.handleEvent(event, *this->context.window);
	optionButton.handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->context.game->popState();
		}

	}
}

void TitleScreenMenuState::update(float deltaTime)
{
	playButton.update(*this->context.window);
	optionButton.update(*this->context.window);
}

void TitleScreenMenuState::draw(sf::RenderWindow& window)
{
	window.draw(this->titleText);
	window.draw(this->playButton);
	window.draw(this->optionButton);
}
