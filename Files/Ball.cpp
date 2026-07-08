#include "Ball.h"
#include "Consts.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Ball::Ball(float x, float y)
{
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color(180,180,180));

    reset(x, y, ServeDirection::Random);
}

void Ball::update(float deltaTime)
{
    shape.move(velocity * deltaTime);

    // Верхняя стенка
    if (shape.getPosition().y <= FIELD_TOP)
    {
        shape.setPosition(
            shape.getPosition().x,
            FIELD_TOP
        );

        velocity.y *= -1;
    }

    // Нижняя стенка
    if (shape.getPosition().y + shape.getRadius() * 2 >= WINDOW_HEIGHT - FIELD_MARGIN)
    {
        shape.setPosition(
            shape.getPosition().x,
            WINDOW_HEIGHT - FIELD_MARGIN - shape.getRadius() * 2
        );

        velocity.y *= -1;
    }
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Ball::isOutLeft() const
{
    return shape.getPosition().x + shape.getRadius() * 2.f < FIELD_LEFT;
}

bool Ball::isOutRight() const
{
    return shape.getPosition().x > FIELD_RIGHT;
}

sf::FloatRect Ball::getBounds() const
{
    return shape.getGlobalBounds();
}

void Ball::reset(float x, float y, ServeDirection direction)
{
    shape.setPosition(x, y);

    const float speed = 430.f;

    // Случайный угол от -60° до +60°
    float angle = (std::rand() % 121 - 60) * 3.14159265f / 180.f;

    switch (direction)
    {
    case ServeDirection::Random:
        if (std::rand() % 2 == 0)
            angle += 3.14159265f;
        break;

    case ServeDirection::Left:
        angle += 3.14159265f;
        break;

    case ServeDirection::Right:
        // Ничего не делаем — угол уже направлен вправо
        break;
    }

    velocity.x = speed * std::cos(angle);
    velocity.y = speed * std::sin(angle);
}

void Ball::checkCollision(const Paddle& paddle, PlayerSide side)
{
    if (!shape.getGlobalBounds().intersects(paddle.getBounds()))
        return;

    lastHitPlayer = side;

    // Центр платформы
    float paddleCenter =
        paddle.getBounds().top +
        paddle.getBounds().height / 2.f;

    // Центр шайбы
    float ballCenter =
        shape.getPosition().y +
        shape.getRadius();

    // От -1 до 1
    float relativeIntersect =
        (ballCenter - paddleCenter) /
        (paddle.getBounds().height / 2.f);

    // Максимальный угол
    const float MAX_ANGLE = 70.f;

    float angle =
        relativeIntersect *
        MAX_ANGLE *
        3.14159265f / 180.f;

    float speed = std::sqrt(
        velocity.x * velocity.x +
        velocity.y * velocity.y
    );

    speed *= 1.03f;


	const float MAX_SPEED = 1200.f;
    
    if (speed > MAX_SPEED)
    {
        speed = MAX_SPEED;
	}

    // Левая платформа
    if (velocity.x < 0)
    {
        velocity.x = speed * std::cos(angle);
    }
    else
    {
        velocity.x = -speed * std::cos(angle);
    }

    velocity.y = speed * std::sin(angle);

    // Чтобы шайба не залипала
    if (velocity.x > 0)
    {
        shape.setPosition(
            paddle.getBounds().left +
            paddle.getBounds().width,
            shape.getPosition().y
        );
    }
    else
    {
        shape.setPosition(
            paddle.getBounds().left -
            shape.getRadius() * 2.f,
            shape.getPosition().y
        );
    }
}

PlayerSide Ball::getLastHitPlayer() const
{
    return lastHitPlayer;
}

void Ball::setSpeed(float newSpeed)
{
    float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (currentSpeed == 0.f)
        return;

    velocity.x = velocity.x / currentSpeed * newSpeed;
    velocity.y = velocity.y / currentSpeed * newSpeed;

    speed = newSpeed;
}

float Ball::getSpeed() const
{
    return speed;
}

void Ball::multiplySpeed(float multiplier)
{
    velocity *= multiplier;
}

void Ball::setSpeedMultiplier(float multiplier)
{
    speedMultiplier = multiplier;
}

float Ball::getSpeedMultiplier() const
{
    return speedMultiplier;
}