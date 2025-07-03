#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <string>

// Helper function to convert integers to strings
std::string to_string(int num);

// Represents a point in the grid
struct Point {
    int x, y;
};

// Represents a single segment of the snake
class SnakeSegment {
public:
    Point position;

    SnakeSegment(int x, int y);
    SnakeSegment();
};

// Represents the grid and other game components
class Grid {
public:
    int rows, cols, cellSize;
    sf::Texture gridTexture, snakeTexture, fruitTexture;
    sf::Sprite gridSprite, snakeSprite, fruitSprite;

    Grid(int rows, int cols, int cellSize);
    void drawGrid(sf::RenderWindow& window);
};

// Manages the game logic and rendering
class SnakeGame {
private:
    Grid grid;
    SnakeSegment snake[100];
    Point fruit;
    int direction, score, highestScore, snakeSize;
    float timer, delay;
    sf::Font font;
    sf::Text scoreText, highestScoreText;
    sf::Text gameOverText;
    bool isGameOver;

// Add to SnakeGame class
int level; // Track current level
float levelDisplayTimer; // Timer for level display
bool showLevelText; // Whether to show "Level" message
sf::Text levelText; // Text to display level
    void spawnFruit();
    bool isFruitOnSnake();
    void updateSnake();
    void resetGame();
    void gameOver();
    void updateScoreText();

public:
    SnakeGame(int rows, int cols, int cellSize);

    void handleInput();
    void update(float timeElapsed);
    void draw(sf::RenderWindow& window);
};

#endif
