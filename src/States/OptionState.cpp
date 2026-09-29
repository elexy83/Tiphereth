#include "States/OptionState.hpp"
#include "Core/Game.hpp"
#include "States/StateIdentifiers.hpp"
#include <algorithm>
#include <functional>

namespace
{
	// Virtual resolution used by the whole UI
	constexpr float ScreenW = 1920.f;
	constexpr float ScreenH = 1080.f;

	// Video tab layout
	constexpr float LabelX = 450.f;
	constexpr float ValueX = 1230.f;
	constexpr float ArrowOffset = 170.f;
	constexpr float VideoRowY[4] = { 340.f, 450.f, 560.f, 670.f };

	// Controls tab layout
	constexpr float GroupSelectorY = 320.f;
	constexpr float HeaderY = 385.f;
	constexpr float RowsStartY = 440.f;
	constexpr float RowStep = 58.f;
	constexpr float Key1X = 1130.f;
	constexpr float Key2X = 1360.f;
	constexpr float ScrollY = 830.f;
	constexpr float HintY = 870.f;

	// Palette
	const sf::Color BtnNormal(60, 63, 85);
	const sf::Color BtnHover(85, 90, 120);
	const sf::Color BtnListening(200, 140, 40);
	const sf::Color BtnListeningHover(225, 165, 60);
	const sf::Color TabActive(80, 110, 190);
	const sf::Color TabActiveHover(100, 130, 210);
	const sf::Color Green(50, 150, 50);
	const sf::Color GreenHover(80, 180, 80);
	const sf::Color Red(150, 50, 50);
	const sf::Color RedHover(180, 80, 80);
	const sf::Color SubtleText(170, 175, 195);

	void styleText(sf::Text& text, const sf::Font& font, unsigned int size, sf::Color color = sf::Color::White)
	{
		text.setFont(font);
		text.setCharacterSize(size);
		text.setFillColor(color);
	}

	void placeText(sf::Text& text, const sf::String& str, float x, float y, bool centered)
	{
		text.setString(str);
		const sf::FloatRect b = text.getLocalBounds();
		text.setOrigin(centered ? b.left + b.width / 2.f : b.left, b.top + b.height / 2.f);
		text.setPosition(x, y);
	}

	void setupButton(GUI::Button& button, const sf::String& text, sf::Vector2f size,
		float x, float y, std::function<void()> callback)
	{
		button.setText(text);
		button.setSize(size);
		button.setPosition(x, y);
		button.setCallback(std::move(callback));
	}

	void setColors(GUI::Button& button, sf::Color normal, sf::Color hover)
	{
		button.setNormalColor(normal);
		button.setHoverColor(hover);
	}
}

OptionState::OptionState(Context context)
	: State(context)
	, tabVideoButton(context.fonts->get(Fonts::ID::Title))
	, tabControlsButton(context.fonts->get(Fonts::ID::Title))
	, backButton(context.fonts->get(Fonts::ID::Title))
	, applyButton(context.fonts->get(Fonts::ID::Title))
	, resetButton(context.fonts->get(Fonts::ID::Title))
	, resPrevButton(context.fonts->get(Fonts::ID::Title))
	, resNextButton(context.fonts->get(Fonts::ID::Title))
	, fpsPrevButton(context.fonts->get(Fonts::ID::Title))
	, fpsNextButton(context.fonts->get(Fonts::ID::Title))
	, fsToggleBtn(context.fonts->get(Fonts::ID::Title))
	, langPrevButton(context.fonts->get(Fonts::ID::Title))
	, langNextButton(context.fonts->get(Fonts::ID::Title))
	, groupPrevButton(context.fonts->get(Fonts::ID::Title))
	, groupNextButton(context.fonts->get(Fonts::ID::Title))
{
	pendingVideoModeIndex = context.game->getCurrentVideoModeIndex();
	pendingFullscreen = context.game->getIsFullscreen();

	const unsigned int currentFPS = context.game->getMaxFPS();
	pendingFpsIndex = 1; // 60 by default
	for (size_t i = 0; i < availableFPS.size(); ++i) {
		if (availableFPS[i] == currentFPS) {
			pendingFpsIndex = static_cast<int>(i);
			break;
		}
	}

	pendingLanguageIndex = 0; // Default "fr"
	const std::string currentLang = context.i18n->getCurrentLanguage();
	for (size_t i = 0; i < availableLanguages.size(); ++i) {
		if (availableLanguages[i] == currentLang) {
			pendingLanguageIndex = static_cast<int>(i);
			break;
		}
	}

	// Backup so "Back" can undo unapplied key changes
	savedBindings = *context.input;

	this->initUI();
	this->updateTexts();
	this->refreshTabStyles();
}

// ----------------------------------------------------------------------
// Construction of the UI (positions, callbacks) - done once
// ----------------------------------------------------------------------

void OptionState::initUI()
{
	sf::Font& font = this->context.fonts->get(Fonts::ID::Title);

	// Dimmed background + centered panel
	overlay.setSize(sf::Vector2f(ScreenW, ScreenH));
	overlay.setFillColor(sf::Color(0, 0, 0, 170));

	panel.setSize(sf::Vector2f(1200.f, 660.f));
	panel.setPosition(360.f, 240.f);
	panel.setFillColor(sf::Color(25, 27, 40, 235));
	panel.setOutlineThickness(2.f);
	panel.setOutlineColor(sf::Color(90, 95, 130));

	styleText(titleText, font, 60);

	// Tabs
	setupButton(tabVideoButton, "", sf::Vector2f(260.f, 50.f), ScreenW / 2.f - 150.f, 190.f,
		[this]() { setTab(Tab::Video); });
	setupButton(tabControlsButton, "", sf::Vector2f(260.f, 50.f), ScreenW / 2.f + 150.f, 190.f,
		[this]() { setTab(Tab::Controls); });

	// ---------------- Video tab ----------------
	for (sf::Text* t : { &resLabel, &fpsLabel, &fsLabel, &langLabel })
		styleText(*t, font, 30);
	for (sf::Text* t : { &resValueText, &fpsValueText, &langValueText })
		styleText(*t, font, 30, sf::Color(255, 220, 120));

	const sf::Vector2f arrowSize(50.f, 50.f);

	setupButton(resPrevButton, "<", arrowSize, ValueX - ArrowOffset, VideoRowY[0], [this]() {
		const int count = static_cast<int>(this->context.game->GetVideoModes().size());
		if (count == 0) return;
		pendingVideoModeIndex = (pendingVideoModeIndex - 1 + count) % count;
		refreshVideoTexts();
		});
	setupButton(resNextButton, ">", arrowSize, ValueX + ArrowOffset, VideoRowY[0], [this]() {
		const int count = static_cast<int>(this->context.game->GetVideoModes().size());
		if (count == 0) return;
		pendingVideoModeIndex = (pendingVideoModeIndex + 1) % count;
		refreshVideoTexts();
		});

	setupButton(fpsPrevButton, "<", arrowSize, ValueX - ArrowOffset, VideoRowY[1], [this]() {
		const int count = static_cast<int>(availableFPS.size());
		pendingFpsIndex = (pendingFpsIndex - 1 + count) % count;
		refreshVideoTexts();
		});
	setupButton(fpsNextButton, ">", arrowSize, ValueX + ArrowOffset, VideoRowY[1], [this]() {
		const int count = static_cast<int>(availableFPS.size());
		pendingFpsIndex = (pendingFpsIndex + 1) % count;
		refreshVideoTexts();
		});

	setupButton(fsToggleBtn, "", sf::Vector2f(200.f, 50.f), ValueX, VideoRowY[2], [this]() {
		pendingFullscreen = !pendingFullscreen;
		refreshVideoTexts();
		});

	setupButton(langPrevButton, "<", arrowSize, ValueX - ArrowOffset, VideoRowY[3], [this]() {
		const int count = static_cast<int>(availableLanguages.size());
		pendingLanguageIndex = (pendingLanguageIndex - 1 + count) % count;
		refreshVideoTexts();
		});
	setupButton(langNextButton, ">", arrowSize, ValueX + ArrowOffset, VideoRowY[3], [this]() {
		const int count = static_cast<int>(availableLanguages.size());
		pendingLanguageIndex = (pendingLanguageIndex + 1) % count;
		refreshVideoTexts();
		});

	for (GUI::Button* b : { &resPrevButton, &resNextButton, &fpsPrevButton, &fpsNextButton,
							&langPrevButton, &langNextButton })
		setColors(*b, BtnNormal, BtnHover);

	// ---------------- Controls tab ----------------
	styleText(groupNameText, font, 34, sf::Color(255, 220, 120));
	for (sf::Text* t : { &headerAction, &headerKey1, &headerKey2, &scrollText })
		styleText(*t, font, 24, SubtleText);
	styleText(hintText, font, 22, SubtleText);

	setupButton(groupPrevButton, "<", arrowSize, ScreenW / 2.f - 250.f, GroupSelectorY, [this]() {
		const std::size_t count = this->context.input->groups().size();
		if (count == 0) return;
		currentGroup = (currentGroup + count - 1) % count;
		scrollOffset = 0;
		refreshControlsTexts();
		});
	setupButton(groupNextButton, ">", arrowSize, ScreenW / 2.f + 250.f, GroupSelectorY, [this]() {
		const std::size_t count = this->context.input->groups().size();
		if (count == 0) return;
		currentGroup = (currentGroup + 1) % count;
		scrollOffset = 0;
		refreshControlsTexts();
		});
	setColors(groupPrevButton, BtnNormal, BtnHover);
	setColors(groupNextButton, BtnNormal, BtnHover);

	rows.resize(MaxVisibleRows);
	for (std::size_t r = 0; r < MaxVisibleRows; ++r) {
		const float y = RowsStartY + static_cast<float>(r) * RowStep;
		styleText(rows[r].label, font, 28);

		for (int s = 0; s < InputManager::SlotCount; ++s) {
			auto button = std::make_unique<GUI::Button>(font);
			button->setSize(sf::Vector2f(200.f, 46.f));
			button->setCharacterSize(24);
			button->setPosition(s == 0 ? Key1X : Key2X, y);
			button->setCallback([this, r, s]() { beginListening(scrollOffset + r, s); });
			setColors(*button, BtnNormal, BtnHover);
			rows[r].slotButtons[s] = std::move(button);
		}
	}

	// ---------------- Bottom buttons ----------------
	const sf::Vector2f bottomSize(260.f, 60.f);
	const float bottomY = 970.f;

	setupButton(backButton, "", bottomSize, ScreenW / 2.f - 300.f, bottomY, [this]() { requestClose(); });
	setupButton(resetButton, "", bottomSize, ScreenW / 2.f, bottomY, [this]() {
		this->context.input->resetGroup(currentGroup);
		stopListening();
		});
	setupButton(applyButton, "", bottomSize, ScreenW / 2.f + 300.f, bottomY, [this]() { applySettings(); });
	setColors(applyButton, Green, GreenHover);

	for (GUI::Button* b : { &backButton, &resetButton, &applyButton })
		b->setCharacterSize(26);
}

// ----------------------------------------------------------------------
// Text refresh (called after any change and after a language switch)
// ----------------------------------------------------------------------

void OptionState::updateTexts()
{
	const auto& i18n = *this->context.i18n;

	placeText(titleText, i18n.get("option.title"), ScreenW / 2.f, 90.f, true);

	tabVideoButton.setText(i18n.get("option.tab.video"));
	tabControlsButton.setText(i18n.get("option.tab.controls"));
	backButton.setText(i18n.get("option.back"));
	applyButton.setText(i18n.get("option.apply"));
	resetButton.setText(i18n.get("option.reset"));

	refreshVideoTexts();
	refreshControlsTexts();
}

void OptionState::refreshVideoTexts()
{
	const auto& i18n = *this->context.i18n;
	const auto& modes = this->context.game->GetVideoModes();

	// Resolution
	placeText(resLabel, i18n.get("option.res"), LabelX, VideoRowY[0], false);
	if (!modes.empty() && pendingVideoModeIndex >= 0
		&& static_cast<std::size_t>(pendingVideoModeIndex) < modes.size())
	{
		const auto& mode = modes[pendingVideoModeIndex];
		placeText(resValueText, std::to_string(mode.width) + "x" + std::to_string(mode.height),
			ValueX, VideoRowY[0], true);
	}

	// FPS
	placeText(fpsLabel, i18n.get("option.fps"), LabelX, VideoRowY[1], false);
	const unsigned int fps = availableFPS[pendingFpsIndex];
	placeText(fpsValueText,
		fps == 0 ? i18n.get("option.unlimited") : sf::String(std::to_string(fps)),
		ValueX, VideoRowY[1], true);

	// Fullscreen
	placeText(fsLabel, i18n.get("option.fs"), LabelX, VideoRowY[2], false);
	fsToggleBtn.setText(i18n.get(pendingFullscreen ? "option.yes" : "option.no"));
	if (pendingFullscreen) setColors(fsToggleBtn, Green, GreenHover);
	else                   setColors(fsToggleBtn, Red, RedHover);

	// Language
	placeText(langLabel, i18n.get("option.lang"), LabelX, VideoRowY[3], false);
	const std::string& langCode = availableLanguages[pendingLanguageIndex];
	std::string langName = langCode;
	if (langCode == "fr") langName = "Francais";
	else if (langCode == "en") langName = "English";
	placeText(langValueText, langName, ValueX, VideoRowY[3], true);
}

void OptionState::refreshControlsTexts()
{
	const auto& i18n = *this->context.i18n;
	const auto& groups = this->context.input->groups();

	for (auto& row : rows) row.visible = false;
	if (groups.empty()) return;

	currentGroup = std::min(currentGroup, groups.size() - 1);
	const auto& group = groups[currentGroup];

	placeText(groupNameText, i18n.get("input.group." + group.id), ScreenW / 2.f, GroupSelectorY, true);
	placeText(headerAction, i18n.get("option.col.action"), LabelX, HeaderY, false);
	placeText(headerKey1, i18n.get("option.col.key1"), Key1X, HeaderY, true);
	placeText(headerKey2, i18n.get("option.col.key2"), Key2X, HeaderY, true);

	const std::size_t total = group.actions.size();
	const std::size_t maxScroll = total > MaxVisibleRows ? total - MaxVisibleRows : 0;
	scrollOffset = std::min(scrollOffset, maxScroll);

	for (std::size_t r = 0; r < MaxVisibleRows; ++r) {
		const std::size_t index = scrollOffset + r;
		if (index >= total) break;

		auto& row = rows[r];
		row.visible = true;

		const auto& action = group.actions[index];
		const float y = RowsStartY + static_cast<float>(r) * RowStep;
		placeText(row.label, i18n.get("input.action." + action.id), LabelX, y, false);

		for (int s = 0; s < InputManager::SlotCount; ++s) {
			const bool listening = isListening()
				&& static_cast<std::size_t>(listeningAction) == index && listeningSlot == s;

			row.slotButtons[s]->setText(listening ? i18n.get("option.listening")
				: bindingToText(action.current[s]));

			if (listening) setColors(*row.slotButtons[s], BtnListening, BtnListeningHover);
			else           setColors(*row.slotButtons[s], BtnNormal, BtnHover);
		}
	}

	// Scroll indicator
	if (maxScroll > 0) {
		const std::size_t last = std::min(scrollOffset + MaxVisibleRows, total);
		placeText(scrollText, std::to_string(scrollOffset + 1) + "-" + std::to_string(last)
			+ " / " + std::to_string(total), ScreenW / 2.f, ScrollY, true);
	}
	else {
		scrollText.setString("");
	}

	placeText(hintText, i18n.get(isListening() ? "option.hint.listening" : "option.hint.controls"),
		ScreenW / 2.f, HintY, true);
}

void OptionState::refreshTabStyles()
{
	const bool video = (currentTab == Tab::Video);
	setColors(tabVideoButton, video ? TabActive : BtnNormal, video ? TabActiveHover : BtnHover);
	setColors(tabControlsButton, video ? BtnNormal : TabActive, video ? BtnHover : TabActiveHover);
}

sf::String OptionState::bindingToText(const InputManager::Binding& binding) const
{
	const auto& i18n = *this->context.i18n;

	switch (binding.type) {
	case InputManager::Binding::Type::Key:
		return sf::String(InputManager::toString(binding));
	case InputManager::Binding::Type::Mouse:
		if (binding.mouse == sf::Mouse::Left)   return i18n.get("input.mouse.left");
		if (binding.mouse == sf::Mouse::Right)  return i18n.get("input.mouse.right");
		if (binding.mouse == sf::Mouse::Middle) return i18n.get("input.mouse.middle");
		if (binding.mouse == sf::Mouse::XButton1) return i18n.get("input.mouse.x1");
		return i18n.get("input.mouse.x2");
	default:
		return i18n.get("input.unbound");
	}
}

// ----------------------------------------------------------------------
// Actions
// ----------------------------------------------------------------------

void OptionState::setTab(Tab tab)
{
	stopListening();
	currentTab = tab;
	refreshTabStyles();
}

void OptionState::applySettings()
{
	// Video
	this->context.game->setCurrentVideoModeIndex(pendingVideoModeIndex);
	this->context.game->setFullScreen(pendingFullscreen);
	this->context.game->setMaxFPS(availableFPS[pendingFpsIndex]);

	this->context.i18n->loadLanguage(availableLanguages[pendingLanguageIndex]);

	const auto& modes = this->context.game->GetVideoModes();
	if (!modes.empty() && pendingVideoModeIndex >= 0
		&& static_cast<std::size_t>(pendingVideoModeIndex) < modes.size())
	{
		this->context.game->setResWidth(modes[pendingVideoModeIndex].width);
		this->context.game->setResHeight(modes[pendingVideoModeIndex].height);
	}
	this->context.game->updateWindow();
	this->context.game->saveSettings(); // persist video + language

	// Controls
	stopListening();
	this->context.input->save();
	savedBindings = *this->context.input;

	this->updateTexts();
}

void OptionState::requestClose()
{
	// Leaving without "Apply": undo unsaved key changes
	*this->context.input = savedBindings;
	requestPop = true;
}

// ----------------------------------------------------------------------
// Key rebinding
// ----------------------------------------------------------------------

void OptionState::beginListening(std::size_t actionIndex, int slot)
{
	listeningAction = static_cast<int>(actionIndex);
	listeningSlot = slot;
	refreshControlsTexts();
}

void OptionState::stopListening()
{
	listeningAction = -1;
	listeningSlot = -1;
	refreshControlsTexts();
}

void OptionState::assignBinding(const InputManager::Binding& binding)
{
	this->context.input->setBinding(currentGroup, static_cast<std::size_t>(listeningAction),
		listeningSlot, binding);
	stopListening();
}

void OptionState::handleBindingCapture(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		const sf::Keyboard::Key code = event.key.code;

		if (code == sf::Keyboard::Escape) {
			stopListening(); // cancel
		}
		else if (code == sf::Keyboard::Delete || code == sf::Keyboard::BackSpace) {
			assignBinding(InputManager::Binding()); // clear the slot
		}
		else if (InputManager::hasKeyName(code)) {
			assignBinding(InputManager::Binding::fromKey(code));
		}
		// Keys we cannot serialize are ignored
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		// GUI::Button fires on release: swallow the release of this click
		if (event.mouseButton.button == sf::Mouse::Left)
			ignoreNextLeftRelease = true;

		assignBinding(InputManager::Binding::fromMouse(event.mouseButton.button));
	}
}

void OptionState::scrollRows(int delta)
{
	const auto& groups = this->context.input->groups();
	if (groups.empty()) return;

	const std::size_t total = groups[currentGroup].actions.size();
	const std::size_t maxScroll = total > MaxVisibleRows ? total - MaxVisibleRows : 0;

	if (delta < 0 && scrollOffset > 0) --scrollOffset;
	else if (delta > 0 && scrollOffset < maxScroll) ++scrollOffset;

	refreshControlsTexts();
}

// ----------------------------------------------------------------------
// State interface
// ----------------------------------------------------------------------

void OptionState::handleEvent(const sf::Event& event)
{
	if (ignoreNextLeftRelease && event.type == sf::Event::MouseButtonReleased
		&& event.mouseButton.button == sf::Mouse::Left)
	{
		ignoreNextLeftRelease = false;
		return;
	}

	// While waiting for a key, everything goes to the capture (Escape = cancel)
	if (isListening()) {
		handleBindingCapture(event);
		return;
	}

	tabVideoButton.handleEvent(event, *this->context.window);
	tabControlsButton.handleEvent(event, *this->context.window);
	backButton.handleEvent(event, *this->context.window);
	applyButton.handleEvent(event, *this->context.window);

	if (currentTab == Tab::Video)
	{
		resPrevButton.handleEvent(event, *this->context.window);
		resNextButton.handleEvent(event, *this->context.window);
		fpsPrevButton.handleEvent(event, *this->context.window);
		fpsNextButton.handleEvent(event, *this->context.window);
		fsToggleBtn.handleEvent(event, *this->context.window);
		langPrevButton.handleEvent(event, *this->context.window);
		langNextButton.handleEvent(event, *this->context.window);
	}
	else
	{
		groupPrevButton.handleEvent(event, *this->context.window);
		groupNextButton.handleEvent(event, *this->context.window);
		resetButton.handleEvent(event, *this->context.window);

		for (auto& row : rows)
			if (row.visible)
				for (auto& button : row.slotButtons)
					button->handleEvent(event, *this->context.window);

		if (event.type == sf::Event::MouseWheelScrolled)
			scrollRows(event.mouseWheelScroll.delta > 0.f ? -1 : 1);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		requestClose();
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

	tabVideoButton.update(*this->context.window);
	tabControlsButton.update(*this->context.window);
	backButton.update(*this->context.window);
	applyButton.update(*this->context.window);

	if (currentTab == Tab::Video)
	{
		resPrevButton.update(*this->context.window);
		resNextButton.update(*this->context.window);
		fpsPrevButton.update(*this->context.window);
		fpsNextButton.update(*this->context.window);
		fsToggleBtn.update(*this->context.window);
		langPrevButton.update(*this->context.window);
		langNextButton.update(*this->context.window);
	}
	else
	{
		groupPrevButton.update(*this->context.window);
		groupNextButton.update(*this->context.window);
		resetButton.update(*this->context.window);

		for (auto& row : rows)
			if (row.visible)
				for (auto& button : row.slotButtons)
					button->update(*this->context.window);
	}
}

void OptionState::draw(sf::RenderWindow& window)
{
	window.draw(overlay);
	window.draw(panel);
	window.draw(titleText);
	window.draw(tabVideoButton);
	window.draw(tabControlsButton);

	if (currentTab == Tab::Video)
	{
		window.draw(resLabel);
		window.draw(resValueText);
		window.draw(fpsLabel);
		window.draw(fpsValueText);
		window.draw(fsLabel);
		window.draw(langLabel);
		window.draw(langValueText);

		window.draw(resPrevButton);
		window.draw(resNextButton);
		window.draw(fpsPrevButton);
		window.draw(fpsNextButton);
		window.draw(fsToggleBtn);
		window.draw(langPrevButton);
		window.draw(langNextButton);
	}
	else
	{
		window.draw(groupNameText);
		window.draw(groupPrevButton);
		window.draw(groupNextButton);
		window.draw(headerAction);
		window.draw(headerKey1);
		window.draw(headerKey2);

		for (auto& row : rows) {
			if (!row.visible) continue;
			window.draw(row.label);
			for (auto& button : row.slotButtons)
				window.draw(*button);
		}

		window.draw(scrollText);
		window.draw(hintText);
		window.draw(resetButton);
	}

	window.draw(backButton);
	window.draw(applyButton);
}