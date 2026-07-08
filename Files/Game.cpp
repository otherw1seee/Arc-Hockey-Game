#include "Game.h"
#include "Consts.h"
#include <stdexcept>
#include <string>
#include "GameState.h"

struct ActiveBonusIcon
{
    BonusType type;
    PlayerSide player;
    float remainingTime;
};

Game::Game()
    :window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arc Hockey"),
    leftPaddle(FIELD_MARGIN + 20.f, 250.f),
    rightPaddle(WINDOW_WIDTH - FIELD_MARGIN - 40.f, 250.f),
    ball(WINDOW_WIDTH / 2.f - 10.f,
        WINDOW_HEIGHT / 2.f - 10.f), bonusManager(font)
    {
    sf::Image icon;
    if (icon.loadFromFile("assets/icon.png"))
    {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    font.loadFromFile("fonts/arial.ttf");
    window.setFramerateLimit(60);
    leftScore = 0;
    rightScore = 0;
    gameState = GameState::Menu;
    const int WIN_SCORE = 10;

    border.setSize(sf::Vector2f(
        WINDOW_WIDTH - FIELD_MARGIN * 2,
        WINDOW_HEIGHT - FIELD_TOP - FIELD_MARGIN
    ));

    border.setPosition(
        FIELD_MARGIN,
        FIELD_TOP
    );
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2.f);
    border.setOutlineColor(sf::Color(230,230,230));

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color(230,230,230));
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        throw std::runtime_error("Не удалось загрузить шрифт!");
    }
    updateScoreText();
    GameState gameState;
    titleText.setFont(font);
    titleText.setCharacterSize(52);
    titleText.setFillColor(sf::Color(240,240,240));
    titleText.setString("ARC HOCKEY");

    sf::FloatRect titleBounds = titleText.getLocalBounds();

    titleText.setOrigin(
        titleBounds.left + titleBounds.width / 2.f,
        titleBounds.top + titleBounds.height / 2.f
    );

    titleText.setPosition(
        WINDOW_WIDTH / 2.f,
        180.f
    );
    menuText.setFont(font);
    menuText.setCharacterSize(30);
    menuText.setFillColor(sf::Color(230,230,230));

    menuText.setString(
        "ENTER - Start Game\n\n"
        "ESC - Exit"
    );

    sf::FloatRect menuBounds = menuText.getLocalBounds();

    menuText.setOrigin(
        menuBounds.left + menuBounds.width / 2.f,
        menuBounds.top + menuBounds.height / 2.f
    );

    menuText.setPosition(
        WINDOW_WIDTH / 2.f,
        340.f
    );
    gameOverTitle.setFont(font);
    gameOverTitle.setCharacterSize(48);
    gameOverTitle.setFillColor(sf::Color(240,240,240));

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(28);
    gameOverText.setFillColor(sf::Color(240,240,240));

    countdownText.setFont(font);
    countdownText.setCharacterSize(150);
    countdownText.setFillColor(sf::Color(240,240,240));

}

void Game::run()
{
    while (window.isOpen()){
        deltaTime = clock.restart().asSeconds();
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
        // Закрытие окна крестиком
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // Нас интересуют только нажатия клавиш
        if (event.type != sf::Event::KeyPressed)
            continue;

        // ESC
        if (event.key.code == sf::Keyboard::Escape)
        {
            if (gameState == GameState::Playing)
            {
                restartGame();
                gameState = GameState::Menu;
            }
            else if (gameState == GameState::Menu)
            {
                window.close();
            }
            else if (gameState == GameState::GameOver)
            {
                gameState = GameState::Menu;
            }

            continue;
        }

        // ENTER в главном меню
        if (gameState == GameState::Menu)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                startCountdown();
            }
        }

        // ENTER после окончания игры
        if (gameState == GameState::GameOver)
        {
            if (event.key.code == sf::Keyboard::Enter)
            {
                restartGame();
            }
        }
    }
}

void Game::update()
{
    if (gameState == GameState::Countdown)
    {
        if (countdownClock.getElapsedTime().asSeconds() >= 1.f)
        {
            countdownNumber--;

            countdownClock.restart();

            if (countdownNumber == 0)
            {
                gameState = GameState::Playing;
            }
            else
            {
                countdownText.setString(
                    std::to_string(countdownNumber)
                );

                sf::FloatRect bounds = countdownText.getLocalBounds();

                countdownText.setOrigin(
                    bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f
                );
            }
        }

        return;
    }

    if (gameState != GameState::Playing)
        return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        leftPaddle.move(-PADDLE_SPEED * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        leftPaddle.move(PADDLE_SPEED * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        rightPaddle.move(-PADDLE_SPEED * deltaTime);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        rightPaddle.move(PADDLE_SPEED * deltaTime);
    }
    leftPaddle.update();
    rightPaddle.update();
    ball.update(deltaTime);
    bonusManager.update();

    Bonus* bonus = bonusManager.getBonus();

    if (bonus)
    {
        if (ball.getBounds().intersects(bonus->getBounds()))
        {
            if (bonus->getType() == BonusType::BiggerPaddle)
            {
                greenBonusPlayer = ball.getLastHitPlayer();

                greenBonusActive = true;

                greenBonusClock.restart();

                if (greenBonusPlayer == PlayerSide::Left)
                    leftPaddle.setHeight(150.f);
                else
                    rightPaddle.setHeight(150.f);
            }

            if (bonus->getType() == BonusType::DoublePoints)
            {

                redBonusActive = true;

                redBonusClock.restart();
            }

            if (bonus->getType() == BonusType::FasterBall)
            {
                blueBonusActive = true;

                blueBonusClock.restart();

                ball.multiplySpeed(1.8f);

            }

            if (bonus->getType() == BonusType::SmallerEnemyPaddle)
            {
                yellowBonusPlayer = ball.getLastHitPlayer();

                yellowBonusActive = true;

                yellowBonusClock.restart();

                if (yellowBonusPlayer == PlayerSide::Left)
                    rightPaddle.setHeightKeepingCenter(SMALL_PADDLE_HEIGHT);
                else
                    leftPaddle.setHeightKeepingCenter(SMALL_PADDLE_HEIGHT);
            }

            bonusManager.removeBonus();
        }
    }

    if (ball.isOutLeft())
    {
        if (redBonusActive)
            rightScore += 2;
        else
            rightScore++;

        updateScoreText();

        ball.reset(
            WINDOW_WIDTH / 2.f - BALL_RADIUS,
            WINDOW_HEIGHT / 2.f - BALL_RADIUS,
            ServeDirection::Left
        );

		redBonusActive = false;
        blueBonusActive = false;
    }

    if (ball.isOutRight())
    {
        if (redBonusActive)
            leftScore += 2;
        else
            leftScore++;

        updateScoreText();

        ball.reset(
            WINDOW_WIDTH / 2.f - BALL_RADIUS,
            WINDOW_HEIGHT / 2.f - BALL_RADIUS,
            ServeDirection::Right
        );
		redBonusActive = false;
        blueBonusActive = false;
        
    }

    if (leftScore >= WIN_SCORE || rightScore >= WIN_SCORE)
    {
        finishGame();
        return;
    }

    if (greenBonusActive)
    {
        if (greenBonusClock.getElapsedTime().asSeconds() >= 8.f)
        {
            greenBonusActive = false;

            leftPaddle.setHeight(PADDLE_HEIGHT);
            rightPaddle.setHeight(PADDLE_HEIGHT);
        }
    }
    if (redBonusActive)
    {
        if (redBonusClock.getElapsedTime().asSeconds() >= 8.f)
        {
            redBonusActive = false;
        }
    }

    if (blueBonusActive)
    {
        if (blueBonusClock.getElapsedTime().asSeconds() >= 8.f)
        {
            blueBonusActive = false;

        }
    }

    if (yellowBonusActive)
    {
        if (yellowBonusClock.getElapsedTime().asSeconds() >= 8.f)
        {
            yellowBonusActive = false;

            leftPaddle.setHeightKeepingCenter(PADDLE_HEIGHT);
            rightPaddle.setHeightKeepingCenter(PADDLE_HEIGHT);
        }
    }

    ball.checkCollision(leftPaddle, PlayerSide::Left);
    ball.checkCollision(rightPaddle, PlayerSide::Right);
}

void Game::render(){
    window.clear(sf::Color(25,25,25));
    if (gameState == GameState::Menu)
    {
        window.draw(titleText);
        window.draw(menuText);
    }
    else if (gameState == GameState::Playing)
    {
        window.draw(border);
        drawDashedCenterLine();

        window.draw(scoreText);

        leftPaddle.draw(window);
        rightPaddle.draw(window);

        ball.draw(window);

        bonusManager.draw(window);
    }
    else if (gameState == GameState::GameOver)
    {
        window.draw(gameOverTitle);
        window.draw(gameOverText);
    }

    else if (gameState == GameState::Countdown)
    {
        window.draw(countdownText);
    }

    drawBonusIcons();

    window.display();
}

void Game::updateScoreText()
{
    scoreText.setString(
        std::to_string(leftScore)
        + " : " +
        std::to_string(rightScore)
    );

    sf::FloatRect bounds = scoreText.getLocalBounds();

    scoreText.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    scoreText.setPosition(
        WINDOW_WIDTH / 2.f,
        15.f
    );
}

void Game::finishGame()
{
    gameState = GameState::GameOver;

    greenBonusActive = false;
    redBonusActive = false;
    blueBonusActive = false;
    yellowBonusActive = false;

    leftPaddle.setHeightKeepingCenter(PADDLE_HEIGHT);
    rightPaddle.setHeightKeepingCenter(PADDLE_HEIGHT);

    if (leftScore >= WIN_SCORE)
    {
        gameOverTitle.setString("LEFT PLAYER WINS!");
    }
    else
    {
        gameOverTitle.setString("RIGHT PLAYER WINS!");
    }

    sf::FloatRect titleBounds = gameOverTitle.getLocalBounds();

    gameOverTitle.setOrigin(
        titleBounds.left + titleBounds.width / 2.f,
        titleBounds.top + titleBounds.height / 2.f
    );

    gameOverTitle.setPosition(
        WINDOW_WIDTH / 2.f,
        180.f
    );

    gameOverText.setString(
        std::to_string(leftScore) +
        " : " +
        std::to_string(rightScore) +
        "\n\nENTER - New Game\n\nESC - Exit"
    );

    sf::FloatRect textBounds = gameOverText.getLocalBounds();

    gameOverText.setOrigin(
        textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f
    );

    gameOverText.setPosition(
        WINDOW_WIDTH / 2.f,
        340.f
    );
}

void Game::restartGame()
{
    leftScore = 0;
    rightScore = 0;

    updateScoreText();

    ball.reset(
        WINDOW_WIDTH / 2.f - BALL_RADIUS,
        WINDOW_HEIGHT / 2.f - BALL_RADIUS,
        ServeDirection::Random
    );

    startCountdown();
}

void Game::startCountdown()
{
    countdownNumber = 3;

    countdownText.setString("3");

    sf::FloatRect bounds = countdownText.getLocalBounds();

    countdownText.setOrigin(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f
    );

    countdownText.setPosition(
        WINDOW_WIDTH / 2.f,
        WINDOW_HEIGHT / 2.f
    );

    countdownClock.restart();

    gameState = GameState::Countdown;
}

void Game::drawBonusIcons()
{
    std::vector<ActiveBonusIcon> icons;

    // Собираем активные бонусы
    if (greenBonusActive)
    {
        icons.push_back({
            BonusType::BiggerPaddle,
            greenBonusPlayer,
            8.f - greenBonusClock.getElapsedTime().asSeconds()
            });
    }

    if (redBonusActive)
    {
        icons.push_back({
            BonusType::DoublePoints,
            redBonusPlayer,
            8.f - redBonusClock.getElapsedTime().asSeconds()
            });
    }

    if (blueBonusActive)
    {
        icons.push_back({
            BonusType::FasterBall,
            PlayerSide::Left,
            8.f - blueBonusClock.getElapsedTime().asSeconds()
            });
    }

    if (yellowBonusActive)
    {
        icons.push_back({
            BonusType::SmallerEnemyPaddle,
            yellowBonusPlayer,
            8.f - yellowBonusClock.getElapsedTime().asSeconds()
            });
    }

    if (icons.empty())
        return;

    const float radius = 20.f;
    const float spacing = 15.f;

    float totalWidth =
        icons.size() * radius * 2.f +
        (icons.size() - 1) * spacing;

    float startX =
        WINDOW_WIDTH / 2.f -
        totalWidth / 2.f +
        radius;

    float y = 55.f;

    for (size_t i = 0; i < icons.size(); i++)
    {
        sf::CircleShape circle(radius);

        circle.setOrigin(radius, radius);

        circle.setPosition(
            startX + i * (radius * 2.f + spacing),
            y
        );

        //-------------------------------------------------
        // Цвет
        //-------------------------------------------------

        switch (icons[i].type)
        {
        case BonusType::BiggerPaddle:
            circle.setFillColor(sf::Color::Green);
            break;

        case BonusType::DoublePoints:
            circle.setFillColor(sf::Color::Red);
            break;

        case BonusType::FasterBall:
            circle.setFillColor(sf::Color::Blue);
            break;

        case BonusType::SmallerEnemyPaddle:
            circle.setFillColor(sf::Color::Yellow);
            break;
        }

        //-------------------------------------------------
        // Символ
        //-------------------------------------------------

        sf::Text text;

        text.setFont(font);

        text.setCharacterSize(22);

        text.setFillColor(sf::Color::Black);

        switch (icons[i].type)
        {
        case BonusType::BiggerPaddle:
            text.setString("+");
            break;

        case BonusType::DoublePoints:
            text.setString("2X");
            break;

        case BonusType::FasterBall:
            text.setString(">>");
            break;

        case BonusType::SmallerEnemyPaddle:
            text.setString("-");
            break;
        }

        sf::FloatRect bounds = text.getLocalBounds();

        text.setOrigin(
            bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f
        );

        text.setPosition(circle.getPosition());

        //-------------------------------------------------
        // Рисуем
        //-------------------------------------------------
        drawBonusTimer(
            circle.getPosition(),
            radius,
            icons[i].remainingTime
        );

        window.draw(circle);
        window.draw(text);
    }
}

void Game::drawBonusTimer(sf::Vector2f center, float radius, float remainingTime)
{
    float percent = remainingTime / 8.f;

    const int segments = 80;

    sf::VertexArray arc(sf::LineStrip);

    float maxAngle = 360.f * percent;

    for (int i = 0; i <= segments * percent; i++)
    {
        float angle = (-90.f + i * (360.f / segments)) * 3.14159265f / 180.f;

        arc.append(sf::Vertex(
            sf::Vector2f(
                center.x + std::cos(angle) * (radius + 5.f),
                center.y + std::sin(angle) * (radius + 5.f)
            ),
            sf::Color(240,240,240)
        ));
    }

    window.draw(arc);
}

void Game::drawDashedCenterLine()
{
    const float dashHeight = 20.f;
    const float gap = 15.f;

    sf::RectangleShape dash;
    dash.setSize({ 4.f, dashHeight });
    dash.setFillColor(sf::Color(70, 70, 70));
    dash.setOrigin(1.f, 0.f);

    const float x = WINDOW_WIDTH / 2.f;

    for (float y = FIELD_TOP + 10.f;
        y <= FIELD_BOTTOM - dashHeight - 10.f;
        y += dashHeight + gap)
    {
        dash.setPosition(x, y);
        window.draw(dash);
    }
}