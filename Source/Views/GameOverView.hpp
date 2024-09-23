#ifndef GAMEOVERVIEW_HPP
#define GAMEOVERVIEW_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "../Utils/LeaderBoard.hpp"


class GameOverView {
public:
    GameOverView(const std::function<void()>& onRestart, const std::function<void()>& onExit);

    void draw(sf::RenderWindow& window, const std::string& playerName, int currentScore, const std::vector<ScoreEntry>& leaderboard);
    void handleMouseClick(int mouseX, int mouseY);

private:
    bool isRestartButtonClicked(int mouseX, int mouseY);
    bool isExitButtonClicked(int mouseX, int mouseY);

    sf::Font font;
    sf::RectangleShape background;
    sf::Text gameOverText;
    sf::Text scoreText;
    sf::Text restartText;
    sf::Text exitText;
    sf::RectangleShape restartButton;
    sf::RectangleShape exitButton;

    std::function<void()> onRestart;
    std::function<void()> onExit;
    
    std::string resourcePath = RESOURCE_DIR;
};

#endif // GAMEOVERVIEW_HPP