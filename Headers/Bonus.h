#pragma once

#include <SFML/Graphics.hpp>
#include "BonusType.h"

class Bonus
{
public:

    Bonus(float x, float y, BonusType type, sf::Font& font);

    void draw(sf::RenderWindow& window, float remainingTime);

    sf::FloatRect getBounds() const;

    BonusType getType() const;

    sf::Text symbol;

  

private:

    sf::CircleShape shape;

    void drawTimer(sf::RenderWindow& window, float remainingTime);

    BonusType type;

};
