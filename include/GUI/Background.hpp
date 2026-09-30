#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>

namespace GUI
{
    /**
     * @brief Full-screen background image, scaled to "cover" the virtual view.
     *
     * The texture is NOT owned: it comes from the TextureManager (loaded once, shared
     * by every state), so the Background only holds a sprite pointing to it.
     * The image keeps its aspect ratio, is centered and cropped if needed. An optional
     * dark layer can be added on top to keep buttons and texts readable.
     */
    class Background : public sf::Drawable
    {
    public:
        Background()
        {
            m_dim.setSize(sf::Vector2f(1920.f, 1080.f));
            m_dim.setFillColor(sf::Color(0, 0, 0, 0));
        }

        /**
         * @brief Uses a texture owned by the TextureManager and fits it to the view.
         *
         * @param texture  Texture that must outlive this Background (the manager guarantees it).
         * @param viewSize Virtual resolution of the game view (1920x1080 by default).
         */
        void setTexture(const sf::Texture& texture, sf::Vector2f viewSize = sf::Vector2f(1920.f, 1080.f))
        {
            m_sprite.setTexture(texture, true);

            const sf::Vector2f texSize(static_cast<float>(texture.getSize().x),
                static_cast<float>(texture.getSize().y));
            const float scale = std::max(viewSize.x / texSize.x, viewSize.y / texSize.y);

            m_sprite.setScale(scale, scale);
            m_sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
            m_sprite.setPosition(viewSize.x / 2.f, viewSize.y / 2.f);

            m_dim.setSize(viewSize);
            m_hasTexture = true;
        }

        /// Darkens the image: 0 = untouched, 255 = fully black.
        void setDim(sf::Uint8 alpha)
        {
            m_dim.setFillColor(sf::Color(0, 0, 0, alpha));
        }

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if (!m_hasTexture) return;
            target.draw(m_sprite, states);
            if (m_dim.getFillColor().a > 0)
                target.draw(m_dim, states);
        }

    private:
        sf::Sprite         m_sprite;
        sf::RectangleShape m_dim;
        bool               m_hasTexture = false;
    };
}