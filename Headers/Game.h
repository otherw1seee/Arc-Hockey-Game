#pragma once

#include <SFML/Graphics.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "GameState.h"
#include "BonusManager.h"

class Game
{
public:
    Game();

    void run();

    sf::Clock greenBonusClock;
    bool greenBonusActive = false;
    PlayerSide greenBonusPlayer;

    sf::Clock redBonusClock;
    bool redBonusActive = false;
    PlayerSide redBonusPlayer;

    sf::Clock blueBonusClock;
    bool blueBonusActive = false;

    sf::Clock yellowBonusClock;
    bool yellowBonusActive = false;
    PlayerSide yellowBonusPlayer;
private:
    void processEvents();
    void update();
    void render();
    void updateScoreText();
    void finishGame();
    void restartGame();
    void startCountdown();
    sf::RenderWindow window;

    sf::Clock clock;

    float deltaTime;

    int leftScore;
    int rightScore;

    GameState gameState;
    sf::Text countdownText;
    sf::Clock countdownClock;
    int countdownNumber;
    sf::RectangleShape centerLine;
    
    sf::RectangleShape border;

    sf::Font font;

    sf::Text scoreText;
    sf::Text titleText;
    sf::Text menuText;
    sf::Text gameOverTitle;
    sf::Text gameOverText;
    Paddle leftPaddle;
    Paddle rightPaddle;
    Ball ball;
    BonusManager bonusManager;

    void drawBonusIcons();

    void drawDashedCenterLine();

    void drawBonusTimer(sf::Vector2f center, float radius, float remainingTime);
};

