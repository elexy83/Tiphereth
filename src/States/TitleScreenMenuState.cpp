#include "States/TitleScreenMenuState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <iostream>

TitleScreenMenuState::TitleScreenMenuState(Context context)
	: State(context)
	, playButton(context.fonts->get(Fonts::ID::Title))
	, optionButton(context.fonts->get(Fonts::ID::Title))
	, quitButton(context.fonts->get(Fonts::ID::Title))
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
	// The callbacks only set a flag: the real work is done in update(),
	// because changeState()/popState() destroy this state while it is still
	// inside handleEvent().

	// Play button
	playButton.setText(this->context.i18n->get("menu.play"));
	playButton.setPosition(1920.f / 2.0f, 1080.f / 2.0f);
	playButton.setCallback([this]()
		{
			this->pendingAction = PendingAction::Play;
		});

	// Option button
	optionButton.setText(this->context.i18n->get("menu.options"));
	optionButton.setPosition(1920.f / 2.0f, 1080.f / 1.5f);
	optionButton.setCallback([this]()
		{
			this->pendingAction = PendingAction::Options;
		});

	// Quit button
	quitButton.setText(this->context.i18n->get("menu.quit"));
	quitButton.setPosition(1920.f / 2.0f, 1080.f / 1.5f + 180.f);
	quitButton.setCallback([this]()
		{
			this->pendingAction = PendingAction::Quit;
		});
}

void TitleScreenMenuState::initBackground()
{
	// The texture is loaded once in Game and shared through the TextureManager
	if (this->context.textures->has(Textures::ID::BackgroundTitle))
	{
		this->background.setTexture(this->context.textures->get(Textures::ID::BackgroundTitle));
		this->background.setDim(90); // 0 = image untouched, 255 = black
	}
}


void TitleScreenMenuState::handleInput()
{
}

void TitleScreenMenuState::handleEvent(const sf::Event& event)
{
	playButton.handleEvent(event, *this->context.window);
	optionButton.handleEvent(event, *this->context.window);
	quitButton.handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
	{
		this->pendingAction = PendingAction::Quit;
	}
}

void TitleScreenMenuState::update(float deltaTime)
{
	// Deferred actions: safe here, nothing is iterating on this state anymore
	if (pendingAction != PendingAction::None)
	{
		const PendingAction action = pendingAction;
		pendingAction = PendingAction::None;

		switch (action)
		{
		case PendingAction::Play:
			this->context.game->changeState(States::ID::ChooseCharacter);
			break;

		case PendingAction::Options:
			// pushState (not changeState) to be able to come back to the title screen
			this->context.game->pushState(States::ID::Option);
			break;

		case PendingAction::Quit:
			this->context.window->close();
			break;

		default:
			break;
		}

		return; // "this" may have been destroyed by changeState(): touch nothing else
	}

	playButton.update(*this->context.window);
	optionButton.update(*this->context.window);
	quitButton.update(*this->context.window);
}

void TitleScreenMenuState::draw(sf::RenderWindow& window)
{
	window.draw(this->background);
	window.draw(this->titleText);
	window.draw(this->playButton);
	window.draw(this->optionButton);
	window.draw(this->quitButton);
}