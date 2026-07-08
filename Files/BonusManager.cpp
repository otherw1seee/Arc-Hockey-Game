#include "BonusManager.h"
#include "Consts.h"
#include <cstdlib>
#include <cmath> 

BonusManager::BonusManager(sf::Font& font)
{
    this->font = &font;

    nextSpawnTime = 8.f + static_cast<float>(rand() % 9);
}

void BonusManager::spawnBonus()
{
    const float BONUS_RADIUS = 30.f;
    const float SAFE_MARGIN = 20.f;

    // Не даем бонусу появиться рядом с ракетками
    float minX = FIELD_LEFT + PADDLE_WIDTH + BONUS_RADIUS + SAFE_MARGIN;
    float maxX = FIELD_RIGHT - PADDLE_WIDTH - BONUS_RADIUS - SAFE_MARGIN;

    // Не даем бонусу выйти за верхнюю и нижнюю границы поля
    float minY = FIELD_TOP + BONUS_RADIUS + SAFE_MARGIN;
    float maxY = FIELD_BOTTOM - BONUS_RADIUS - SAFE_MARGIN;

    float x =
        minX +
        std::rand() % int(maxX - minX);

    float y =
        minY +
        std::rand() % int(maxY - minY);

    BonusType type =
        static_cast<BonusType>(
            std::rand() % 4
            );

    currentBonus =
        std::make_unique<Bonus>(x, y, type, *font);

    lifeClock.restart();
}

void BonusManager::update()
{
    if (!currentBonus)
    {
        if (spawnClock.getElapsedTime().asSeconds() >= nextSpawnTime)
        {
            spawnBonus();
            nextSpawnTime = 8.f + static_cast<float>(std::rand() % 9);

            spawnClock.restart();
        }
    }
    else
    {
        if (lifeClock.getElapsedTime().asSeconds() >= 8.f)
        {
            currentBonus.reset();

            spawnClock.restart();
            nextSpawnTime = 8.f + static_cast<float>(std::rand() % 9);
        }
    }
}

void BonusManager::draw(sf::RenderWindow& window)
{
    if (currentBonus) {
        float remaining =
            8.f -
            lifeClock.getElapsedTime().asSeconds();

        currentBonus->draw(window, remaining);
    }
}

Bonus* BonusManager::getBonus()
{
    return currentBonus.get();
}

void BonusManager::removeBonus()
{
    currentBonus.reset();
    spawnClock.restart();
    nextSpawnTime = 8.f + static_cast<float>(std::rand() % 9);
}