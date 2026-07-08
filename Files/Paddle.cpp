#include "Paddle.h"
#include "Consts.h"

Paddle::Paddle(float x, float y)
{
    shape.setSize(sf::Vector2f(20.f, 100.f));

    shape.setPosition(x, y);

    shape.setFillColor(sf::Color(240,240,240));

    shape.setOutlineThickness(1.5f);
    shape.setOutlineColor(sf::Color(170, 170, 170));
}

void Paddle::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}
void Paddle::move(float dy)
{
    shape.move(0.f, dy);
}

void Paddle::update()
{
    if (shape.getPosition().y < FIELD_TOP)
    {
        shape.setPosition(shape.getPosition().x, FIELD_TOP);
    }

    if (shape.getPosition().y + shape.getSize().y > WINDOW_HEIGHT - FIELD_MARGIN)
    {
        shape.setPosition(
            shape.getPosition().x,
            WINDOW_HEIGHT - FIELD_MARGIN - shape.getSize().y
        );
    }
}

sf::FloatRect Paddle::getBounds() const
{
    return shape.getGlobalBounds();
}

void Paddle::setHeight(float height)
{
    shape.setSize({ PADDLE_WIDTH, height });
}

float Paddle::getHeight() const
{
    return shape.getSize().y;
}

void Paddle::setHeightKeepingCenter(float newHeight)
{
    // Запоминаем центр платформы
    sf::Vector2f center = shape.getPosition();
    center.y += shape.getSize().y / 2.f;

    // Меняем размер
    shape.setSize({ PADDLE_WIDTH, newHeight });

    // Возвращаем центр на прежнее место
    shape.setPosition(
        shape.getPosition().x,
        center.y - newHeight / 2.f
    );
}