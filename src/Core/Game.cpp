#include "Core/Game.hpp"
#include "States/TitleScreenMenuState.hpp"

Game::Game()
{
    this->initWindow();
    this->initView();
    this->initState();
}

void Game::initWindow()
{
    this->videoModes = sf::VideoMode::getFullscreenModes();

    this->currentVideoModeIndex = 0;
    this->resWidth = this->videoModes[this->currentVideoModeIndex].width;
    this->resHeight = this->videoModes[this->currentVideoModeIndex].height;

    this->updateWindow();
}

void Game::initView()
{
    this->view = sf::View(sf::FloatRect(0.f, 0.f, VIEW_WIDTH, VIEW_HEIGHT));
    this->window.setView(this->view);
}

void Game::initState()
{
    this->pushState(std::make_unique<TitleScreenMenuState>(this));
}

void Game::pushState(std::unique_ptr<State> state)
{
    this->states.push(std::move(state));
}

void Game::popState()
{
    if (!this->states.empty())
    {
        this->states.pop();
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

void Game::changeState(std::unique_ptr<State> state)
{
    if (!this->states.empty())
    {
        this->states.pop();
    }
    this->states.push(std::move(state));
}

void Game::updateWindow()
{
    sf::Uint32 style = this->isFullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    this->window.create(sf::VideoMode(this->resWidth, this->resHeight), "Shoot'em up", style);

    this->window.setView(this->view);
}

void Game::run()
{

    sf::Clock clock;
    while (this->window.isOpen())
    {
        this->deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (this->window.pollEvent(event))
        {
            // if the window is closed
            if (event.type == sf::Event::Closed)
            {
                this->window.close();
            }

            // Give to the top state the events
            if (!this->states.empty())
            {
                this->states.top()->handleEvent(event);
            }

            // Recaculate the viewport
            if (event.type == sf::Event::Resized)
            {
                // Comparating viewport with the window
                float windowRatio = static_cast<float>(event.size.width) / static_cast<float>(event.size.height);
                float viewRatio = VIEW_WIDTH / VIEW_HEIGHT;

                float sizeX = 1.f;
                float sizeY = 1.f;
                float posX = 0.f;
                float posY = 0.f;

                // if it's too wide (adding black stripes on the edges)
                if (windowRatio > viewRatio)
                {
                    sizeX = viewRatio / windowRatio;
                    posX = (1.f - sizeX) / 2.f;
                }
                // if it's too high (adding black stripes on the top and bottom)
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
