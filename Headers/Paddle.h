#pragma once

#include <SFML/Graphics.hpp>

class Paddle
{
public:
    Paddle(float x, float y);

    void draw(sf::RenderWindow& window);
    void move(float dy);
    void update();
    sf::FloatRect getBounds() const;
    void setHeight(float height);
    float getHeight() const;
    void setHeightKeepingCenter(float newHeight);

private:
    sf::RectangleShape shape;
};
