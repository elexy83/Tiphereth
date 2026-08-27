#include "States/OptionState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <iostream>

OptionState::OptionState(Context context)
	: State(context)
	, applyButton(context.fonts->get(Fonts::ID::Title))
	, backButton(context.fonts->get(Fonts::ID::Title))
	, quitButton(context.fonts->get(Fonts::ID::Title))
	, resPrevButton(context.fonts->get(Fonts::ID::Title))
	, resNextButton(context.fonts->get(Fonts::ID::Title))
	, fsToggleBtn(context.fonts->get(Fonts::ID::Title))
	, requestPop(false)
	, requestQuit(false)
{
	pendingVideoModeIndex = context.game->getCurrentVideoModeIndex();
	pendingFullscreen = context.game->getIsFullscreen();

	this->initUI();
	this->updateTexts();
}

void OptionState::initUI()
{
	sf::Font& font = this->context.fonts->get(Fonts::ID::Title);

	// Title
	titleText.setFont(font);
	titleText.setString("OPTIONS");
	titleText.setCharacterSize(70);
	titleText.setFillColor(sf::Color::White);
	sf::FloatRect textRect = titleText.getLocalBounds();
	titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	titleText.setPosition(1920.f / 2.f, 150.f);

	// Resolution
	float row1_Y = 400.f;

	resLabel.setFont(font);
	resLabel.setString("Resolution :");
	resLabel.setCharacterSize(40);
	sf::FloatRect resLblRect = resLabel.getLocalBounds();
	resLabel.setOrigin(resLblRect.left, resLblRect.top + resLblRect.height / 2.f);
	resLabel.setPosition(500.f, row1_Y);

	resValueText.setFont(font);
	resValueText.setCharacterSize(40);

	resPrevButton.setText("<");
	resPrevButton.setSize(sf::Vector2f(60.f, 60.f));
	resPrevButton.setPosition(1300.f - 200.f, row1_Y);
	resPrevButton.setCallback([this]()
	{
		pendingVideoModeIndex--;
		if (pendingVideoModeIndex < 0) {
			pendingVideoModeIndex = this->context.game->GetVideoModes().size() - 1;
		}
		this->updateTexts();
	});

	resNextButton.setText(">");
	resNextButton.setSize(sf::Vector2f(60.f, 60.f));
	resNextButton.setPosition(1300.f + 200.f, row1_Y);
	resNextButton.setCallback([this]()
	{
		pendingVideoModeIndex++;
		if (pendingVideoModeIndex >= this->context.game->GetVideoModes().size()) {
			pendingVideoModeIndex = 0;
		}
		this->updateTexts();
	});

	//  Fullscreen
	float row2_Y = 550.f;

	fsLabel.setFont(font);
	fsLabel.setString("Plein Ecran :");
	fsLabel.setCharacterSize(40);
	sf::FloatRect fsLblRect = fsLabel.getLocalBounds();
	fsLabel.setOrigin(fsLblRect.left, fsLblRect.top + fsLblRect.height / 2.f);
	fsLabel.setPosition(500.f, row2_Y);

	fsToggleBtn.setSize(sf::Vector2f(200.f, 60.f));
	fsToggleBtn.setPosition(1300.f, row2_Y);
	fsToggleBtn.setCallback([this]() {
		pendingFullscreen = !pendingFullscreen;
		this->updateTexts();
		});

	// Action button
	float buttonY = 850.f;

	// Back to the previous state
	backButton.setText("Retour");
	backButton.setSize(sf::Vector2f(300.f, 80.f));
	backButton.setPosition(1920.f / 4.f, buttonY);
	backButton.setCallback([this]()
	{
		requestPop = true;
	});

	// Apply button
	applyButton.setText("Appliquer");
	applyButton.setSize(sf::Vector2f(300.f, 80.f));
	applyButton.setPosition(1920.f / 2.f, buttonY);
	applyButton.setNormalColor(sf::Color(50, 150, 50));
	applyButton.setHoverColor(sf::Color(80, 180, 80));
	applyButton.setCallback([this]()
	{
		this->applySettings();
	});

	// Close the game
	quitButton.setText("Quitter le jeu");
	quitButton.setSize(sf::Vector2f(350.f, 80.f));
	quitButton.setPosition(1920.f * 0.75f, buttonY);
	quitButton.setNormalColor(sf::Color(150, 50, 50));
	quitButton.setHoverColor(sf::Color(180, 80, 80));
	quitButton.setCallback([this]()
	{
		requestQuit = true;
	});
}

void OptionState::updateTexts()
{
	const auto& modes = this->context.game->GetVideoModes();
	if (!modes.empty() && pendingVideoModeIndex >= 0 && pendingVideoModeIndex < modes.size())
	{
		std::string resString = std::to_string(modes[pendingVideoModeIndex].width) + "x" +
			std::to_string(modes[pendingVideoModeIndex].height);
		resValueText.setString(resString);

		sf::FloatRect bounds = resValueText.getLocalBounds();
		resValueText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		resValueText.setPosition(1300.f, 400.f);
	}

	if (pendingFullscreen) {
		fsToggleBtn.setText("OUI");
		fsToggleBtn.setNormalColor(sf::Color(50, 150, 50)); 
	}
	else {
		fsToggleBtn.setText("NON");
		fsToggleBtn.setNormalColor(sf::Color(150, 50, 50));
	}
}

void OptionState::applySettings()
{
	this->context.game->setCurrentVideoModeIndex(pendingVideoModeIndex);
	this->context.game->setFullScreen(pendingFullscreen);

	const auto& modes = this->context.game->GetVideoModes();
	if (!modes.empty() && pendingVideoModeIndex >= 0 && pendingVideoModeIndex < modes.size()) {
		this->context.game->setResWidth(modes[pendingVideoModeIndex].width);
		this->context.game->setResHeight(modes[pendingVideoModeIndex].height);
	}
	this->context.game->updateWindow();
}

void OptionState::handleEvent(const sf::Event& event)
{
	applyButton.handleEvent(event, *this->context.window);
	backButton.handleEvent(event, *this->context.window);
	quitButton.handleEvent(event, *this->context.window);
	resPrevButton.handleEvent(event, *this->context.window);
	resNextButton.handleEvent(event, *this->context.window);
	fsToggleBtn.handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->context.game->popState();
		}
	}
}

void OptionState::handleInput()
{
}

void OptionState::update(float deltaTime)
{
	if (requestPop) {
		this->context.game->popState();
		return;
	}
	if (requestQuit) {
		this->context.window->close();
		return;
	}
	applyButton.update(*this->context.window);
	backButton.update(*this->context.window);
	quitButton.update(*this->context.window);
	resPrevButton.update(*this->context.window);
	resNextButton.update(*this->context.window);
	fsToggleBtn.update(*this->context.window);
}

void OptionState::draw(sf::RenderWindow& window)
{

	window.draw(titleText);

	window.draw(resLabel);
	window.draw(resValueText);
	window.draw(fsLabel);

	window.draw(applyButton);
	window.draw(backButton);
	window.draw(quitButton);
	window.draw(resPrevButton);
	window.draw(resNextButton);
	window.draw(fsToggleBtn);
}