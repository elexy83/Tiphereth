#pragma once

#include <SFML/Graphics.hpp>
#include "Managers/ResourceManager.hpp"

class Game;

struct Context {

	sf::RenderWindow* window;
	Game* game;

	// Managers
	TextureManager* textures;
	FontManager* fonts;

	Context(
		sf::RenderWindow* win = nullptr,
		Game* g = nullptr,
		TextureManager* tex = nullptr,
		FontManager* f = nullptr
	) : window(win), game(g), textures(tex), fonts(f) {}
};