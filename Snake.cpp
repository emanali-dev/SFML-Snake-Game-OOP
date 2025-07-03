#include "Snake.h"
#include <sstream>
#include <time.h>

// Helper function to convert integers to strings
std::string to_string(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

// SnakeSegment methods
SnakeSegment::SnakeSegment(int x, int y) {
    position.x = x;
    position.y = y;
}

SnakeSegment::SnakeSegment() {
    position.x = -1;
    position.y = -1;
}

// Grid methods
Grid::Grid(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize) {
    gridTexture.loadFromFile("back1.png");
    snakeTexture.loadFromFile("Snake.png");
    fruitTexture.loadFromFile("food.png");

    gridSprite.setTexture(gridTexture);
    snakeSprite.setTexture(snakeTexture);
    fruitSprite.setTexture(fruitTexture);
}

void Grid::drawGrid(sf::RenderWindow& window) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            gridSprite.setPosition(i * cellSize, j * cellSize);
            window.draw(gridSprite);
        }
    }
}

// SnakeGame methods
SnakeGame::SnakeGame(int rows, int cols, int cellSize)
    : grid(rows, cols, cellSize), direction(0), score(0), highestScore(0),
      snakeSize(2), timer(0), delay(0.1), isGameOver(false) {
    srand(time(0));
    spawnFruit();

    snake[0] = SnakeSegment(10, 10);
    snake[1] = SnakeSegment(10, 11);

    font.loadFromFile("font1.ttf");
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setCharacterSize(18);

    highestScoreText.setFont(font);
    highestScoreText.setFillColor(sf::Color::White);
    highestScoreText.setPosition(400, 10);
    highestScoreText.setCharacterSize(18);

    gameOverText.setFont(font);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setCharacterSize(30);
    gameOverText.setString("");
}

void SnakeGame::spawnFruit() {
    do {
        fruit.x = rand() % grid.cols;
        fruit.y = rand() % grid.rows;
    } while (isFruitOnSnake());
}

bool SnakeGame::isFruitOnSnake() {
    for (int i = 0; i < snakeSize; i++) {
        if (snake[i].position.x == fruit.x && snake[i].position.y == fruit.y) {
            return true;
        }
    }
    return false;
}

void SnakeGame::updateSnake() {
    if (snakeSize > 0) {
        for (int i = snakeSize; i > 0; --i) {
            snake[i].position = snake[i - 1].position;
        }

        if (direction == 0) snake[0].position.y += 1;
        if (direction == 1) snake[0].position.x -= 1;
        if (direction == 2) snake[0].position.x += 1;
        if (direction == 3) snake[0].position.y -= 1;

        if (snake[0].position.x == fruit.x && snake[0].position.y == fruit.y) {
            snakeSize++;
            score++;
            if (score > highestScore) highestScore = score;
            spawnFruit();
        }

        static int previousScore = -1;
        if (score % 5 == 0 && score != 0 && score != previousScore) {
            delay -= 0.01;
            previousScore = score;
        }

        if (snake[0].position.x >= grid.cols) snake[0].position.x = 0;
        if (snake[0].position.x < 0) snake[0].position.x = grid.cols - 1;
        if (snake[0].position.y >= grid.rows) snake[0].position.y = 0;
        if (snake[0].position.y < 0) snake[0].position.y = grid.rows - 1;

        for (int i = 1; i < snakeSize; i++) {
            if (snake[0].position.x == snake[i].position.x &&
                snake[0].position.y == snake[i].position.y) {
                gameOver();
                break;
            }
        }
    }
}

void SnakeGame::resetGame() {
    snakeSize = 2;
    snake[0] = SnakeSegment(10, 10);
    snake[1] = SnakeSegment(10, 11);
    score = 0;
    delay = 0.1;
    spawnFruit();
    isGameOver = false;
}

void SnakeGame::gameOver() {
    isGameOver = true;
    gameOverText.setString("Game Over! Press R to Restart");
    gameOverText.setPosition(grid.cols * grid.cellSize / 4, grid.rows * grid.cellSize / 2);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Red);
}

void SnakeGame::updateScoreText() {
    scoreText.setString("Current Score: " + to_string(score));
    highestScoreText.setString("Highest Score: " + to_string(highestScore));
}

void SnakeGame::handleInput() {
    if (isGameOver) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
            resetGame();
            gameOverText.setString("");
        }
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 2) direction = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 1) direction = 2;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 0) direction = 3;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 3) direction = 0;
}

void SnakeGame::update(float timeElapsed) {
    if (!isGameOver) {
        timer += timeElapsed;
        if (timer > delay) {
            timer = 0;
            updateSnake();
        }
    }
    updateScoreText();
}

void SnakeGame::draw(sf::RenderWindow& window) {
    grid.drawGrid(window);

    for (int i = 0; i < snakeSize; i++) {
        grid.snakeSprite.setPosition(snake[i].position.x * grid.cellSize,
            snake[i].position.y * grid.cellSize);
        window.draw(grid.snakeSprite);
    }

    grid.fruitSprite.setPosition(fruit.x * grid.cellSize,
        fruit.y * grid.cellSize);
    window.draw(grid.fruitSprite);

    window.draw(scoreText);
    window.draw(highestScoreText);
    window.draw(gameOverText);
}
