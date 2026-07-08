#pragma once

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "ServeDirection.h"
#include "PlayerSide.h"

class Ball
{
public:
    Ball(float x, float y);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

    void checkCollision(const Paddle& paddle, PlayerSide side);

    sf::FloatRect getBounds() const;

    bool isOutLeft() const;
    bool isOutRight() const;

    void reset(float x, float y, ServeDirection direction);
    PlayerSide getLastHitPlayer() const;
    void setSpeed(float newSpeed);

    float getSpeed() const;

    void multiplySpeed(float multiplier);

    void setSpeedMultiplier(float multiplier);
    float getSpeedMultiplier() const;
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    PlayerSide lastHitPlayer = PlayerSide::Left;
    float speed = 450.f;
    float speedMultiplier = 1.f;
};
