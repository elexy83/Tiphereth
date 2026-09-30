#include "States/ChooseCharacterState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <string>

namespace
{
	constexpr float ScreenW = 1920.f;
}

ChooseCharacterState::ChooseCharacterState(Context context)
	: State(context)
{
	this->initUI();
}

void ChooseCharacterState::initUI()
{
	sf::Font& font = this->context.fonts->get(Fonts::ID::Title);
	const auto& i18n = *this->context.i18n;

	// Background shared through the TextureManager
	if (this->context.textures->has(Textures::ID::BackgroundTitle))
	{
		this->background.setTexture(this->context.textures->get(Textures::ID::BackgroundTitle));
		this->background.setDim(120);
	}

	// Title
	this->titleText.setFont(font);
	this->titleText.setCharacterSize(60);
	this->titleText.setFillColor(sf::Color::White);
	this->titleText.setString(i18n.get("choose.title"));
	const sf::FloatRect tb = this->titleText.getLocalBounds();
	this->titleText.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
	this->titleText.setPosition(ScreenW / 2.f, 140.f);

	// 4 big buttons in a row, centered
	const float buttonW = 380.f;
	const float buttonH = 520.f;
	const float gap = 40.f;
	const float totalW = CharacterCount * buttonW + (CharacterCount - 1) * gap;
	const float firstX = (ScreenW - totalW) / 2.f + buttonW / 2.f;

	for (std::size_t i = 0; i < CharacterCount; ++i)
	{
		auto button = std::make_unique<GUI::Button>(font);
		button->setSize(sf::Vector2f(buttonW, buttonH));
		button->setPosition(firstX + static_cast<float>(i) * (buttonW + gap), 580.f);
		button->setCharacterSize(34);
		button->setText(i18n.get("choose.character." + std::to_string(i + 1)));

		// Only sets a flag: changing/pushing states is done in update()
		button->setCallback([this, i]()
			{
				this->pendingCharacter = static_cast<int>(i);
				this->pendingAction = PendingAction::OpenShop;
			});

		this->characterButtons[i] = std::move(button);
	}
}

void ChooseCharacterState::handleInput()
{
}

void ChooseCharacterState::handleEvent(const sf::Event& event)
{
	for (auto& button : this->characterButtons)
		button->handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		this->pendingAction = PendingAction::Back;
}

void ChooseCharacterState::update(float deltaTime)
{
	if (this->pendingAction != PendingAction::None)
	{
		const PendingAction action = this->pendingAction;
		this->pendingAction = PendingAction::None;

		switch (action)
		{
		case PendingAction::OpenShop:
			this->context.game->setSelectedCharacter(this->pendingCharacter);
			this->context.game->pushState(States::ID::Shop); // pushed: Escape in the shop comes back here
			break;

		case PendingAction::Back:
			this->context.game->changeState(States::ID::TitleScreen);
			break;

		default:
			break;
		}
		return; // "this" may be destroyed by changeState(): touch nothing else
	}

	for (auto& button : this->characterButtons)
		button->update(*this->context.window);
}

void ChooseCharacterState::draw(sf::RenderWindow& window)
{
	window.draw(this->background);
	window.draw(this->titleText);
	for (auto& button : this->characterButtons)
		window.draw(*button);
}