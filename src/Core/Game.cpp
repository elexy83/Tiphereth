#include "Core/Game.hpp"
#include "States/TitleScreenMenuState.hpp"
#include "States/OptionState.hpp"
#include "States/ChooseCharacterState.hpp"
#include <iostream>

namespace
{
	const char* const SettingsPath = "assets/config/settings.json";
}

Game::Game()
{
	this->initWindow();
	this->initView();
	this->initContext();
	this->i18n.loadLanguage("fr");

	registerDefaultBindings(this->inputManager);
	this->inputManager.load();

	this->initState();
}

void Game::initWindow()
{
	this->videoModes = sf::VideoMode::getFullscreenModes();

	this->currentVideoModeIndex = 0;
	this->resWidth = this->videoModes[this->currentVideoModeIndex].width;
	this->resHeight = this->videoModes[this->currentVideoModeIndex].height;

	this->loadSettings(); // override the defaults above with the saved values

	this->updateWindow();
}

void Game::loadSettings()
{
	std::ifstream file(SettingsPath);
	if (!file.is_open()) return; // first launch: keep the defaults

	std::stringstream buffer;
	buffer << file.rdbuf();
	const std::string content = buffer.str();

	std::unordered_map<std::string, std::string> values;
	const std::regex pattern(R"(\"([^"]+)\"\s*:\s*\"?([^",}\s]*)\"?)");
	for (auto it = std::sregex_iterator(content.begin(), content.end(), pattern);
		it != std::sregex_iterator(); ++it)
	{
		values[(*it)[1].str()] = (*it)[2].str();
	}

	// Resolution: only restored if this monitor still offers it
	if (values.count("width") && values.count("height"))
	{
		try {
			const unsigned int w = static_cast<unsigned int>(std::stoul(values["width"]));
			const unsigned int h = static_cast<unsigned int>(std::stoul(values["height"]));
			for (std::size_t i = 0; i < this->videoModes.size(); ++i)
			{
				if (this->videoModes[i].width == w && this->videoModes[i].height == h)
				{
					this->currentVideoModeIndex = static_cast<int>(i);
					this->resWidth = w;
					this->resHeight = h;
					break;
				}
			}
		}
		catch (...) {}
	}

	if (values.count("fullscreen"))
		this->isFullscreen = (values["fullscreen"] == "true");

	if (values.count("max_fps"))
	{
		try { this->maxFPS = static_cast<unsigned int>(std::stoul(values["max_fps"])); }
		catch (...) {}
	}

	if (values.count("language") && !values["language"].empty())
		this->i18n.loadLanguage(values["language"]);
}

void Game::saveSettings()
{
	std::error_code ec;
	std::filesystem::create_directories(std::filesystem::path(SettingsPath).parent_path(), ec);

	std::ofstream file(SettingsPath);
	if (!file.is_open())
	{
		std::cerr << "Game::saveSettings - Impossible d'ecrire : " << SettingsPath << std::endl;
		return;
	}

	file << "{\n"
		<< "  \"width\": " << this->resWidth << ",\n"
		<< "  \"height\": " << this->resHeight << ",\n"
		<< "  \"fullscreen\": " << (this->isFullscreen ? "true" : "false") << ",\n"
		<< "  \"max_fps\": " << this->maxFPS << ",\n"
		<< "  \"language\": \"" << this->i18n.getCurrentLanguage() << "\"\n"
		<< "}\n";
}

void Game::initView()
{
	this->view = sf::View(sf::FloatRect(0.f, 0.f, VIEW_WIDTH, VIEW_HEIGHT));
	this->window.setView(this->view);
}

void Game::initState()
{
	this->pushState(States::ID::TitleScreen);
}

void Game::initContext()
{
	this->context.window = &this->window;
	this->context.game = this;
	this->context.textures = &this->textures;
	this->context.fonts = &this->fonts;
	this->context.i18n = &this->i18n;
	this->context.input = &this->inputManager;

	try {
		this->context.fonts->load(Fonts::ID::Title, "assets/fonts/Orbitron/static/Orbitron-Regular.ttf");
	}
	catch (std::exception& e) {
		std::cerr << "ERREUR CRITIQUE : " << e.what() << std::endl;
	}
}

void Game::pushState(States::ID stateID)
{
	this->states.push(this->createState(stateID));
}

void Game::popState()
{
	if (!this->states.empty())
	{
		this->states.pop();
	}
}

std::unique_ptr<State> Game::createState(States::ID stateID)
{
	switch (stateID)
	{
	case States::ID::TitleScreen:
		return std::make_unique<TitleScreenMenuState>(this->context);

	case States::ID::Option:
		return std::make_unique<OptionState>(this->context);

	case States::ID::ChooseCharacter:
		return std::make_unique<ChooseCharacterState>(this->context);

		// case States::ID::Game:
		//	return std::make_unique<GameState>(this->context);

	default:
		return nullptr;
	}
}

const unsigned int Game::getResWidth()
{
	return this->resWidth;
}

const unsigned int Game::getResHeight()
{
	return this->resHeight;
}

const bool Game::getIsFullscreen()
{
	return this->isFullscreen;
}

const int Game::getCurrentVideoModeIndex()
{
	return this->currentVideoModeIndex;
}

const std::vector<sf::VideoMode> Game::GetVideoModes()
{
	return this->videoModes;
}

const bool Game::GetIsInGame()
{
	return this->isInGame;
}

void Game::setFullScreen(bool isFullscreen)
{
	this->isFullscreen = isFullscreen;
}

void Game::setResWidth(unsigned int width)
{
	this->resWidth = width;
}

void Game::setResHeight(unsigned int height)
{
	this->resHeight = height;
}

void Game::setCurrentVideoModeIndex(int index)
{
	this->currentVideoModeIndex = index;
}

void Game::setIsInGame(bool isInGame)
{
	this->isInGame = isInGame;
}

// Nouvelles fonctions pour récupérer et appliquer la limite de FPS
const unsigned int Game::getMaxFPS()
{
	return this->maxFPS;
}

void Game::setMaxFPS(unsigned int fps)
{
	this->maxFPS = fps;
	this->window.setFramerateLimit(this->maxFPS);
}

void Game::nextVideoMode()
{
	this->currentVideoModeIndex++;

	if (this->currentVideoModeIndex >= this->videoModes.size())
	{
		this->currentVideoModeIndex = 0;
	}

	this->resWidth = this->videoModes[this->currentVideoModeIndex].width;
	this->resHeight = this->videoModes[this->currentVideoModeIndex].height;
}

void Game::changeState(States::ID stateID)
{
	if (!this->states.empty())
	{
		this->states.pop();
	}
	this->states.push(this->createState(stateID));
}

void Game::updateWindow()
{
	static sf::Uint32 currentStyle = -1;
	sf::Uint32 newStyle = this->isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;

	bool styleChanged = (currentStyle != newStyle);
	bool resChanged = (this->window.getSize().x != this->resWidth || this->window.getSize().y != this->resHeight);

	if (styleChanged || (newStyle == sf::Style::Fullscreen && resChanged) || !this->window.isOpen())
	{
		this->window.create(sf::VideoMode(this->resWidth, this->resHeight), "Tiphereth", newStyle);
		currentStyle = newStyle;

		// Le code anti-flash blanc est bien ici !
		this->window.clear(sf::Color::Black);
		this->window.display();

		// On applique la limite dynamique
		this->window.setFramerateLimit(this->maxFPS);
	}
	else if (resChanged)
	{
		this->window.setSize(sf::Vector2u(this->resWidth, this->resHeight));
	}

	this->window.setView(this->view);
}

void Game::run()
{
	sf::Clock clock;
	float fpsTimer = 0.0f;
	int frameCount = 0;

	while (this->window.isOpen())
	{
		this->deltaTime = clock.restart().asSeconds(); // DeltaTime Calculation

		// DEBUG : fps calculation
		fpsTimer += this->deltaTime;
		frameCount++;
		if (fpsTimer >= 1.0f) {
			std::cout << "[DEBUG] FPS Actuel : " << frameCount << std::endl;
			fpsTimer -= 1.0f;
			frameCount = 0;
		}

		sf::Event event;
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)  // If the window is closed
			{
				this->window.close();
			}

			if (!this->states.empty()) // Give to the top state the events
			{
				this->states.top()->handleEvent(event);
			}

			if (event.type == sf::Event::Resized) // Recalculate the viewport
			{
				float windowRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
				float viewRatio = VIEW_WIDTH / VIEW_HEIGHT;

				float sizeX = 1.f;
				float sizeY = 1.f;
				float posX = 0.f;
				float posY = 0.f;

				if (windowRatio > viewRatio)
				{
					sizeX = viewRatio / windowRatio;
					posX = (1.f - sizeX) / 2.f;
				}
				else
				{
					sizeY = windowRatio / viewRatio;
					posY = (1.f - sizeY) / 2.f;
				}

				this->view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
				this->window.setView(this->view);
			}
		}

		if (!this->states.empty())
		{
			this->states.top()->handleInput();
		}

		if (!this->states.empty())
		{
			this->states.top()->update(this->deltaTime);
		}

		if (!this->states.empty())
		{
			this->window.clear(sf::Color::Black);
			this->states.top()->draw(this->window);
			this->window.display();
		}
		else
		{
			this->window.close();
		}
	}
}