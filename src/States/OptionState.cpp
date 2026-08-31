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
	, langPrevButton(context.fonts->get(Fonts::ID::Title))
	, langNextButton(context.fonts->get(Fonts::ID::Title))
	, fpsPrevButton(context.fonts->get(Fonts::ID::Title))
	, fpsNextButton(context.fonts->get(Fonts::ID::Title))
	, requestPop(false)
	, requestQuit(false)
{

	pendingVideoModeIndex = context.game->getCurrentVideoModeIndex();
	pendingFullscreen = context.game->getIsFullscreen();

	unsigned int currentFPS = context.game->getMaxFPS();
	pendingFpsIndex = 1; // 60 by default
	for (size_t i = 0; i < availableFPS.size(); ++i) {
		if (availableFPS[i] == currentFPS) {
			pendingFpsIndex = static_cast<int>(i);
			break;
		}
	}

	pendingLanguageIndex = 0; // Default "fr"
	std::string currentLang = this->context.i18n->getCurrentLanguage();
	for (size_t i = 0; i < availableLanguages.size(); ++i) {
		if (availableLanguages[i] == currentLang) {
			pendingLanguageIndex = static_cast<int>(i);
			break;
		}
	}

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
	titleText.setString(this->context.i18n->get("option.title"));
	sf::FloatRect textRect = titleText.getLocalBounds();
	titleText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	titleText.setPosition(1920.f / 2.f, 150.f);

	float row1_Y = 320.f; // Resolution
	float row2_Y = 470.f; // FPS
	float row3_Y = 620.f; // Fullscreen
	float row4_Y = 770.f; // Langue

	// Resolution
	resLabel.setFont(font);
	resLabel.setString(this->context.i18n->get("option.res"));
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
				pendingVideoModeIndex = static_cast<int>(this->context.game->GetVideoModes().size() - 1);
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


	// FPS
	fpsLabel.setFont(font);
	fpsLabel.setString(this->context.i18n->get("option.fps"));
	fpsLabel.setCharacterSize(40);
	sf::FloatRect fpsLblRect = fpsLabel.getLocalBounds();
	fpsLabel.setOrigin(fpsLblRect.left, fpsLblRect.top + fpsLblRect.height / 2.f);
	fpsLabel.setPosition(500.f, row2_Y);

	fpsValueText.setFont(font);
	fpsValueText.setCharacterSize(40);

	fpsPrevButton.setText("<");
	fpsPrevButton.setSize(sf::Vector2f(60.f, 60.f));
	fpsPrevButton.setPosition(1300.f - 200.f, row2_Y);
	fpsPrevButton.setCallback([this]()
		{
			pendingFpsIndex--;
			if (pendingFpsIndex < 0) {
				pendingFpsIndex = static_cast<int>(availableFPS.size()) - 1;
			}
			this->updateTexts();
		});

	fpsNextButton.setText(">");
	fpsNextButton.setSize(sf::Vector2f(60.f, 60.f));
	fpsNextButton.setPosition(1300.f + 200.f, row2_Y);
	fpsNextButton.setCallback([this]()
		{
			pendingFpsIndex++;
			if (pendingFpsIndex >= static_cast<int>(availableFPS.size())) {
				pendingFpsIndex = 0;
			}
			this->updateTexts();
		});


	// Fullscreen
	fsLabel.setFont(font);
	fsLabel.setString(this->context.i18n->get("option.fs"));
	fsLabel.setCharacterSize(40);
	sf::FloatRect fsLblRect = fsLabel.getLocalBounds();
	fsLabel.setOrigin(fsLblRect.left, fsLblRect.top + fsLblRect.height / 2.f);
	fsLabel.setPosition(500.f, row3_Y);

	fsToggleBtn.setSize(sf::Vector2f(200.f, 60.f));
	fsToggleBtn.setPosition(1300.f, row3_Y);
	fsToggleBtn.setCallback([this]() {
		pendingFullscreen = !pendingFullscreen;
		this->updateTexts();
		});


	// Languages
	langLabel.setFont(font);
	langLabel.setString(this->context.i18n->get("option.lang"));
	langLabel.setCharacterSize(40);
	sf::FloatRect langLblRect = langLabel.getLocalBounds();
	langLabel.setOrigin(langLblRect.left, langLblRect.top + langLblRect.height / 2.f);
	langLabel.setPosition(500.f, row4_Y);

	langValueText.setFont(font);
	langValueText.setCharacterSize(40);

	langPrevButton.setText("<");
	langPrevButton.setSize(sf::Vector2f(60.f, 60.f));
	langPrevButton.setPosition(1300.f - 200.f, row4_Y);
	langPrevButton.setCallback([this]()
		{
			pendingLanguageIndex--;
			if (pendingLanguageIndex < 0) {
				pendingLanguageIndex = static_cast<int>(availableLanguages.size()) - 1;
			}
			this->updateTexts();
		});

	langNextButton.setText(">");
	langNextButton.setSize(sf::Vector2f(60.f, 60.f));
	langNextButton.setPosition(1300.f + 200.f, row4_Y);
	langNextButton.setCallback([this]()
		{
			pendingLanguageIndex++;
			if (pendingLanguageIndex >= static_cast<int>(availableLanguages.size())) {
				pendingLanguageIndex = 0;
			}
			this->updateTexts();
		});


	// Action button
	float buttonY = 920.f;

	// Back to the previous state
	backButton.setText(this->context.i18n->get("option.back"));
	backButton.setSize(sf::Vector2f(300.f, 80.f));
	backButton.setPosition(1920.f / 4.f, buttonY);
	backButton.setCallback([this]()
		{
			requestPop = true;
		});

	// Apply button
	applyButton.setText(this->context.i18n->get("option.apply"));
	applyButton.setSize(sf::Vector2f(300.f, 80.f));
	applyButton.setPosition(1920.f / 2.f, buttonY);
	applyButton.setNormalColor(sf::Color(50, 150, 50));
	applyButton.setHoverColor(sf::Color(80, 180, 80));
	applyButton.setCallback([this]()
		{
			this->applySettings();
		});

	// Close the game
	quitButton.setText(this->context.i18n->get("menu.quit"));
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
		resValueText.setPosition(1300.f, 320.f);
	}

	unsigned int fps = availableFPS[pendingFpsIndex];
	if (fps == 0) {
		fpsValueText.setString(this->context.i18n->get("option.unlimited"));
	}
	else {
		fpsValueText.setString(std::to_string(fps));
	}
	sf::FloatRect fpsBounds = fpsValueText.getLocalBounds();
	fpsValueText.setOrigin(fpsBounds.left + fpsBounds.width / 2.0f, fpsBounds.top + fpsBounds.height / 2.0f);
	fpsValueText.setPosition(1300.f, 470.f);


	if (pendingFullscreen) {
		fsToggleBtn.setText(this->context.i18n->get("option.yes"));
		fsToggleBtn.setNormalColor(sf::Color(50, 150, 50));
	}
	else {
		fsToggleBtn.setText(this->context.i18n->get("option.no"));
		fsToggleBtn.setNormalColor(sf::Color(150, 50, 50));
	}


	std::string langCode = availableLanguages[pendingLanguageIndex];
	if (langCode == "fr") langValueText.setString("Francais");
	else if (langCode == "en") langValueText.setString("English");
	else langValueText.setString(langCode);

	sf::FloatRect langBounds = langValueText.getLocalBounds();
	langValueText.setOrigin(langBounds.left + langBounds.width / 2.0f, langBounds.top + langBounds.height / 2.0f);
	langValueText.setPosition(1300.f, 770.f);

	titleText.setString(this->context.i18n->get("option.title"));
	sf::FloatRect titleBounds = titleText.getLocalBounds();
	titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f); // Recentrage du titre

	resLabel.setString(this->context.i18n->get("option.res"));
	fsLabel.setString(this->context.i18n->get("option.fs"));
	langLabel.setString(this->context.i18n->get("option.lang"));

	backButton.setText(this->context.i18n->get("option.back"));
	applyButton.setText(this->context.i18n->get("option.apply"));
	quitButton.setText(this->context.i18n->get("menu.quit"));
}

void OptionState::applySettings()
{
	this->context.game->setCurrentVideoModeIndex(pendingVideoModeIndex);
	this->context.game->setFullScreen(pendingFullscreen);

	this->context.game->setMaxFPS(availableFPS[pendingFpsIndex]);

	std::string selectedLang = availableLanguages[pendingLanguageIndex];
	this->context.i18n->loadLanguage(selectedLang);

	const auto& modes = this->context.game->GetVideoModes();
	if (!modes.empty() && pendingVideoModeIndex >= 0 && pendingVideoModeIndex < modes.size()) {
		this->context.game->setResWidth(modes[pendingVideoModeIndex].width);
		this->context.game->setResHeight(modes[pendingVideoModeIndex].height);
	}
	this->context.game->updateWindow();

	this->updateTexts();
}

void OptionState::handleEvent(const sf::Event& event)
{
	applyButton.handleEvent(event, *this->context.window);

	backButton.handleEvent(event, *this->context.window);

	quitButton.handleEvent(event, *this->context.window);

	resPrevButton.handleEvent(event, *this->context.window);
	resNextButton.handleEvent(event, *this->context.window);

	fpsPrevButton.handleEvent(event, *this->context.window);
	fpsNextButton.handleEvent(event, *this->context.window);

	fsToggleBtn.handleEvent(event, *this->context.window);

	langPrevButton.handleEvent(event, *this->context.window);
	langNextButton.handleEvent(event, *this->context.window);

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->requestPop = true;
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

	fpsPrevButton.update(*this->context.window);
	fpsNextButton.update(*this->context.window);

	fsToggleBtn.update(*this->context.window);

	langPrevButton.update(*this->context.window);
	langNextButton.update(*this->context.window);
}

void OptionState::draw(sf::RenderWindow& window)
{

	window.draw(titleText);
	window.draw(resLabel);
	window.draw(resValueText);
	window.draw(fpsLabel);
	window.draw(fpsValueText);
	window.draw(fsLabel);
	window.draw(langLabel);
	window.draw(langValueText);

	window.draw(applyButton);
	window.draw(backButton);
	window.draw(quitButton);
	window.draw(resPrevButton);
	window.draw(resNextButton);
	window.draw(fpsPrevButton);
	window.draw(fpsNextButton);
	window.draw(fsToggleBtn);

	window.draw(langPrevButton);
	window.draw(langNextButton);
}