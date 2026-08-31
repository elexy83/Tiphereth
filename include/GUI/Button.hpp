#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

/**
 * @brief User Interface namespace containing reusable graphical widgets.
 */
namespace GUI
{
    /**
     * @brief A reusable, interactive GUI button widget for SFML.
     *
     * The Button class inherits from sf::Drawable and sf::Transformable, allowing it
     * to be easily positioned, scaled, rotated, and drawn directly onto an sf::RenderTarget.
     * It handles mouse hover visual feedback and click callbacks via std::function.
     */
    class Button : public sf::Drawable, public sf::Transformable
    {
    public:
        /// Type alias for the button click callback function handler.
        using Callback = std::function<void()>;

    public:
        /**
         * @brief Constructs a new Button instance with a specific font.
         *
         * @param font The sf::Font reference to use for rendering the button's text.
         */
        Button(const sf::Font& font);

        /**
         * @brief Sets the callback function to execute when the button is clicked.
         *
         * @param callback A std::function<void()> or lambda expression containing the action.
         */
        void setCallback(Callback callback);

        /**
         * @brief Sets the text displayed inside the button and centers it automatically.
         *
         * @param text The string content to display.
         */
        void setText(const sf::String& text);

        /**
         * @brief Sets the width and height dimensions of the button shape.
         *
         * @param size An sf::Vector2f representing the new width and height in pixels.
         */
        void setSize(sf::Vector2f size);

        /**
         * @brief Sets the default background color of the button when not hovered.
         *
         * @param color An sf::Color value.
         */
        void setNormalColor(sf::Color color);

        /**
         * @brief Sets the background color of the button when the mouse is hovering over it.
         *
         * @param color An sf::Color value.
         */
        void setHoverColor(sf::Color color);

        /**
         * @brief Sets the text fill color.
         *
         * @param color An sf::Color value.
         */
        void setTextColor(sf::Color color);

        /**
         * @brief Checks whether the mouse cursor is currently positioned over the button.
         *
         * @param window Reference to the active sf::RenderWindow used to map pixel coordinates.
         * @return true If the mouse cursor is inside the button's global bounds.
         * @return false Otherwise.
         */
        bool isMouseOver(sf::RenderWindow& window) const;

        /**
         * @brief Updates the button state each frame (e.g., handling hover color changes).
         *
         * @param window Reference to the active sf::RenderWindow.
         */
        void update(sf::RenderWindow& window);

        /**
         * @brief Handles incoming SFML events to detect left mouse button releases.
         *
         * @param event Constant reference to the sf::Event being processed.
         * @param window Reference to the active sf::RenderWindow.
         */
        void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    private:
        /**
         * @brief Overridden draw method from sf::Drawable to render the button onto a target.
         *
         * @param target The render target to draw onto (e.g., sf::RenderWindow).
         * @param states The render states to apply (transformations, views, etc.).
         */
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        /**
         * @brief Helper utility to perfectly center the text origin inside the button shape.
         */
        void centerText();

    private:
        /// The function callback triggered upon clicking the button.
        Callback            m_callback;

        /// The background rectangle shape of the button.
        sf::RectangleShape  m_shape;

        /// The text label rendered over the button shape.
        sf::Text            m_text;

        /// The base color of the button shape when idle.
        sf::Color           m_normalColor;

        /// The highlight color of the button shape when hovered.
        sf::Color           m_hoverColor;
    };
}