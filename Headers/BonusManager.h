#pragma once

#include <memory>
#include <SFML/Graphics.hpp>

#include "Bonus.h"

class BonusManager
{
public:

    BonusManager(sf::Font& font);

    void update();

    float nextSpawnTime;

    void draw(sf::RenderWindow& window);

    Bonus* getBonus();

    void removeBonus();

    sf::Font* font;
private:

    std::unique_ptr<Bonus> currentBonus;

    sf::Clock spawnClock;
    sf::Clock lifeClock;

    void spawnBonus();
    
};