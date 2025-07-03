#include "Snake.h"
#include <SFML/Graphics.hpp>

int main() {
    const int N = 40, M = 30, size = 14;

    sf::RenderWindow window(sf::VideoMode(size * N, size * M), "Snake Game!");
    SnakeGame game(M, N, size);

    sf::Clock clock;

    while (window.isOpen()) {
        float timeElapsed = clock.restart().asSeconds();

        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
        }

        game.handleInput();
        game.update(timeElapsed);

        window.clear();
        game.draw(window);
        window.display();
    }

    return 0;
}
