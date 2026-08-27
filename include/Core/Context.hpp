#pragma once

#include <SFML/Graphics.hpp>
#include "Managers/ResourceManager.hpp"

class Game;

/**
 * @brief Bundles global resources and engine references for easy sharing across game states.
 *
 * The Context struct acts as a lightweight "toolbox" passed down to every State
 * constructor. It grants controlled access to the main render window, the core
 * game instance, and asset managers without requiring global variables.
 */
struct Context {

    /// Pointer to the main application render window.
    sf::RenderWindow* window;

    /// Pointer to the core Game engine instance.
    Game* game;

    /// Pointer to the texture asset manager repository.
    TextureManager* textures;

    /// Pointer to the font asset manager repository.
    FontManager* fonts;

    /**
     * @brief Constructs a new Context toolbox with optional initial pointers.
     *
     * @param win Pointer to the main sf::RenderWindow.
     * @param g Pointer to the main Game engine instance.
     * @param tex Pointer to the TextureManager repository.
     * @param f Pointer to the FontManager repository.
     */
    Context(
        sf::RenderWindow* win = nullptr,
        Game* g = nullptr,
        TextureManager* tex = nullptr,
        FontManager* f = nullptr
    ) : window(win), game(g), textures(tex), fonts(f) {
    }
};