#include "States/GameState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <string>

GameState::GameState(Context context)
	: State(context)
{
	sf::Font& font = this->context.fonts->get(Fonts::ID::Title);
	const auto& i18n = *this->context.i18n;
	const int gameplay = this->context.game->getSelectedCharacter() + 1;

	this->infoText.setFont(font);
	this->infoText.setCharacterSize(70);
	this->infoText.setFillColor(sf::Color::White);
	this->infoText.setString(i18n.get("game.placeholder") + sf::String(" " + std::to_string(gameplay)));
	sf::FloatRect b = this->infoText.getLocalBounds();
	this->infoText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
	this->infoText.setPosition(1920.f / 2.f, 1080.f / 2.f);

	this->hintText.setFont(font);
	this->hintText.setCharacterSize(28);
	this->hintText.setFillColor(sf::Color(170, 175, 195));
	this->hintText.setString(i18n.get("game.hint"));
	b = this->hintText.getLocalBounds();
	this->hintText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
	this->hintText.setPosition(1920.f / 2.f, 1080.f / 2.f + 90.f);
}

void GameState::handleInput()
{
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		this->requestPop = true;
}

void GameState::update(float deltaTime)
{
	if (this->requestPop)
	{
		this->context.game->popState(); // back to the character selection
		return;
	}
}

void GameState::draw(sf::RenderWindow& window)
{
	window.draw(this->infoText);
	window.draw(this->hintText);
}