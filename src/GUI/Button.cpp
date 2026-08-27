#include "GUI/Button.hpp"

namespace GUI
{
    Button::Button(const sf::Font& font)
        : m_normalColor(sf::Color(100, 100, 100))  // Gray by default
        , m_hoverColor(sf::Color(150, 150, 150))   // Light gray on hover
    {
        m_text.setFont(font);
        m_text.setCharacterSize(30);
        m_text.setFillColor(sf::Color::White);

        m_shape.setFillColor(m_normalColor);
        m_shape.setSize(sf::Vector2f(300.f, 80.f)); // Défault size
        sf::FloatRect bounds = m_shape.getLocalBounds();
        m_shape.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    }

    void Button::setCallback(Callback callback)
    {
        m_callback = callback;
    }

    void Button::setText(const std::string& text)
    {
        m_text.setString(text);
        centerText();
    }

    void Button::setSize(sf::Vector2f size)
    {
        m_shape.setSize(size);
        sf::FloatRect bounds = m_shape.getLocalBounds();
        m_shape.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        centerText();
    }

    void Button::setNormalColor(sf::Color color)
    {
        m_normalColor = color;
        m_shape.setFillColor(m_normalColor);
    }

    void Button::setHoverColor(sf::Color color)
    {
        m_hoverColor = color;
    }

    void Button::setTextColor(sf::Color color)
    {
        m_text.setFillColor(color);
    }

    bool Button::isMouseOver(sf::RenderWindow& window) const
    {
        sf::Vector2i mousePosWindow = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosWindow);
        return getTransform().transformRect(m_shape.getGlobalBounds()).contains(mousePosWorld);
    }

    void Button::update(sf::RenderWindow& window)
    {
        if (isMouseOver(window))
        {
            m_shape.setFillColor(m_hoverColor);
        }
        else
        {
            m_shape.setFillColor(m_normalColor);
        }
    }

    void Button::handleEvent(const sf::Event& event, sf::RenderWindow& window)
    {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            if (isMouseOver(window))
            {
                if (m_callback)
                {
                    m_callback();
                }
            }
        }
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        target.draw(m_shape, states);
        target.draw(m_text, states);
    }

    void Button::centerText()
    {
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        m_text.setPosition(0.f, 0.f);
    }
}