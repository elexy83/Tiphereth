#pragma once

#include <SFML/Graphics.hpp>

class Game;

struct Context {

	sf::RenderWindow* window;
	Game* game;

	// Future Managers ici
	// 
	// TextureManager* textures;
	// FontManager* fonts;


	Context(sf::RenderWindow* win = nullptr, Game* g = nullptr) : window(win), game(g) {}
};