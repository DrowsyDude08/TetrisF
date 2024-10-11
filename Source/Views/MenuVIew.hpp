#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP
#pragma warning
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Utils/ColorPalette.hpp"
#include "../Utils/LeaderBoard.hpp"
#include "../Utils/GameStates.hpp"
#include "../Controllers/GameController.hpp"


class MenuView {
public:
    MenuView(sf::RenderWindow& window);


    void drawMainMenu();
    void drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore);

    void handleMainMenuMouseMove(int mouseX, int mouseY, GameState gameState);
    void handleMainMenuKeyboardInput(const sf::Event& event);

    //void handleGameOverMouseMove(int mouseX, int mouseY);
    //void handleGameOverKeyboardInput(const sf::Event& event);

    bool isStartButtonClicked(int mouseX, int mouseY);
    bool isChangeColorButtonClicked(int mouseX, int mouseY);
    bool isExitButtonClicked(int mouseX, int mouseY);
    bool isGameOverReplayClicked(int mouseX, int mouseY);
    bool isGameOverToMainMenuClicked(int mouseX, int mouseY);

    void handleMainMenuEvent(const sf::Event& event);

    int selectedButtonIndex;
private:
    sf::RenderWindow& window;

    sf::RectangleShape mainMenuStartButton;
    sf::RectangleShape mainMenuChangeColorButton;
    sf::RectangleShape mainMenuExitButton;

    sf::RectangleShape gameOverReplayButton;
    sf::RectangleShape gameOverToMainMenuButton;

    GameState currentState;


    sf::Font font;
    sf::Text mainMenuStartText;
    sf::Text mainMenuChangeColorText;
    sf::Text mainMenuExitText;

    sf::Text gameOverReplayText;
    sf::Text gameOverToMainMenuText;

    ColorPalette palette;
    sf::RectangleShape* hoveredButton;

    std::string resourcePath = RESOURCE_DIR;

    void initializeButtons();

    void updateMainMenuButtonAppearance();
    void selectMainMenuButton(int index);

    void selectGameOverButton(int index);
    void updateGameOverButtonAppearance();

    void swapTextures();
    std::string texturePiecePath = resourcePath + "/Sprites/jstris1.png";
    std::string textureGhostPath = resourcePath + "/Sprites/ghost.png";
};

#endif // MENUVIEW_HPP