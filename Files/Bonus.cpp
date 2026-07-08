#include "Bonus.h"

Bonus::Bonus(float x, float y, BonusType type, sf::Font& font)
{
    this->type = type;

    shape.setRadius(30.f);

    shape.setOrigin(30.f, 30.f);

    shape.setPosition(x, y);

    shape.setPosition(x, y);

    switch (type)
    {
    case BonusType::BiggerPaddle:
        shape.setFillColor(sf::Color::Green);
        break;

    case BonusType::DoublePoints:
        shape.setFillColor(sf::Color::Red);
        break;

    case BonusType::FasterBall:
        shape.setFillColor(sf::Color::Blue);
        break;

    case BonusType::SmallerEnemyPaddle:
        shape.setFillColor(sf::Color::Yellow);
        break;
    }

    symbol.setFont(font);

    symbol.setCharacterSize(20);

    symbol.setFillColor(sf::Color::Black);

    switch (type)
    {
    case BonusType::BiggerPaddle:

        symbol.setString("+");

        break;

    case BonusType::DoublePoints:

        symbol.setString("2x");

        break;

    case BonusType::FasterBall:

        symbol.setString(">>");

        break;

    case BonusType::SmallerEnemyPaddle:

        symbol.setString("-");

        break;
    }

    sf::FloatRect bounds =
        symbol.getLocalBounds();

    symbol.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    symbol.setPosition(x, y);
}

void Bonus::draw(sf::RenderWindow& window, float remainingTime)
{
    drawTimer(window, remainingTime);

    window.draw(shape);

    window.draw(symbol);
}

sf::FloatRect Bonus::getBounds() const
{
    return shape.getGlobalBounds();
}

BonusType Bonus::getType() const
{
    return type;
}

void Bonus::drawTimer(sf::RenderWindow& window, float remainingTime)
{
    float percent = remainingTime / 8.f;

    const int segments = 80;

    sf::VertexArray arc(sf::LineStrip);

    sf::Vector2f center = shape.getPosition();

    float radius = shape.getRadius() + 5.f;

    for (int i = 0; i <= segments * percent; i++)
    {
        float angle =
            (-90.f + i * (360.f / segments)) *
            3.14159265f / 180.f;

        arc.append(sf::Vertex(
            {
                center.x + std::cos(angle) * radius,
                center.y + std::sin(angle) * radius
            },
            sf::Color(240,240,240)
        ));
    }

    window.draw(arc);
}