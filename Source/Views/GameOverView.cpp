#include "GameOverView.hpp"

GameOverView::GameOverView(const std::function<void()>& onRestart, const std::function<void()>& onExit)
    : onRestart(onRestart), onExit(onExit) {
    if (!font.loadFromFile(resourcePath + "/Fonts/ShoraiSansStdNVar.ttf")) {
        throw std::runtime_error("Failed to load font for GameOverView");
    }

    background.setFillColor(sf::Color(0, 0, 0, 150));
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");

    scoreText.setFont(font);
    restartText.setFont(font);
    restartText.setString("Restart");
    exitText.setFont(font);
    exitText.setString("Exit");

    restartButton.setSize(sf::Vector2f(200, 50));
    exitButton.setSize(sf::Vector2f(200, 50));
}

void GameOverView::draw(sf::RenderWindow& window, const std::string& playerName, int currentScore, const std::vector<ScoreEntry>& leaderboard) {
    window.draw(background);

    gameOverText.setCharacterSize(50);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, 100);

    window.draw(gameOverText);

    scoreText.setString("Score: " + std::to_string(currentScore));
    scoreText.setCharacterSize(30); 
    scoreText.setPosition(window.getSize().x / 2 - scoreText.getGlobalBounds().width / 2, 200);
    window.draw(scoreText);

    restartText.setCharacterSize(30); 
    restartText.setPosition(window.getSize().x / 2 - restartText.getGlobalBounds().width / 2, 300);
    window.draw(restartText);

    exitText.setCharacterSize(30);
    exitText.setPosition(window.getSize().x / 2 - exitText.getGlobalBounds().width / 2, 370);
    window.draw(exitText);

    restartButton.setPosition(window.getSize().x / 2 - restartButton.getSize().x / 2, 300);
    exitButton.setPosition(window.getSize().x / 2 - exitButton.getSize().x / 2, 370);

    window.draw(restartButton);
    window.draw(exitButton);
}
