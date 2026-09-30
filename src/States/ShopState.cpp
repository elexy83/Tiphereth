#include "States/ShopState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <string>

ShopState::ShopState(Context context)
	: State(context)
	, playButton(context.fonts->get(Fonts::ID::Title))
{
	this->initUI();
}

void ShopState::initUI()
{
	sf::Font& font = this->context.fonts->get(Fonts::ID::Title);
	const auto& i18n = *this->context.i18n;

	if (this->context.textures->has(Textures::ID::BackgroundTitle))
	{
		this->background.setTexture(this->context.textures->get(Textures::ID::BackgroundTitle));
		this->background.setDim(150);
	}

	// Title: "Shop - Character N"
	const int character = this->context.game->getSelectedCharacter();
	this->titleText.setFont(font);
	this->titleText.setCharacterSize(60);
	this->titleText.setFillColor(sf::Color::White);
	this->titleText.setString(i18n.get("shop.title") + sf::String(" - ")
		+ i18n.get("choose.character." + std::to_string(character + 1)));
	const sf::FloatRect tb = this->titleText.getLocalBounds();
	this->titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
	this->titleText.setPosition(1920.f / 2.f, 140.f);

	// "Play" button, bottom left (position = center of the button)
	this->playButton.setSize(sf::Vector2f(300.f, 80.f));
	this->playButton.setPosition(250.f, 1000.f);
	this->playButton.setText(i18n.get("shop.play"));
	this->playButton.setNormalColor(sf::Color(50, 150, 50));
	this->playButton.setHoverColor(sf::Color(80, 180, 80));
	this->playButton.setCallback([this]()
		{
			this->pendingAction = PendingAction::Play;
		});
}

void ShopState::handleInput()
{
}

void ShopState::handleEvent(const sf::Event& event)
{
	this->playButton.handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		this->pendingAction = PendingAction::Back;
}

void ShopState::update(float deltaTime)
{
	if (this->pendingAction != PendingAction::None)
	{
		const PendingAction action = this->pendingAction;
		this->pendingAction = PendingAction::None;

		switch (action)
		{
		case PendingAction::Play:
			// Replaces the shop: the stack becomes [ChooseCharacter, Game]
			this->context.game->changeState(States::ID::Game);
			break;

		case PendingAction::Back:
			this->context.game->popState(); // back to the character selection
			break;

		default:
			break;
		}
		return; // "this" is destroyed: touch nothing else
	}

	this->playButton.update(*this->context.window);
}

void ShopState::draw(sf::RenderWindow& window)
{
	window.draw(this->background);
	window.draw(this->titleText);
	window.draw(this->playButton);
}