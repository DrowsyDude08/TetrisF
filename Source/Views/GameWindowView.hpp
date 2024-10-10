#ifndef GAMEWINDOWVIEW_HPP
#define GAMEWINDOWVIEW_HPP

#include <SFML/Graphics.hpp>
#include "../Utils/ColorPalette.hpp"
#include "../Utils/LeaderBoard.hpp"
#include "../Utils/GameStates.hpp"

class GameWindowView {
public:
    GameWindowView(sf::RenderWindow& window);

    void drawMainMenu();
    void drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore);

    void handleMainMenuMouseMove(int mouseX, int mouseY, GameState gameState);
    void handleMainMenuKeyboardInput(const sf::Event& event);
    //void handleSaveButtonClick();

    //void handlePauseMouseMove(int mouseX, int mouseY);
    //void handlePauseKeyboardInput(const sf::Event& event);

    //void handleGameOverMouseMove(int mouseX, int mouseY);
    //void handleGameOverKeyboardInput(const sf::Event& event);

    bool isStartButtonClicked(int mouseX, int mouseY);
    bool isChangeColorButtonClicked(int mouseX, int mouseY);
    bool isExitButtonClicked(int mouseX, int mouseY);
    bool isGameOverReplayClicked(int mouseX, int mouseY);
    bool isGameOverToMainMenuClicked(int mouseX, int mouseY);
    //bool isPauseContinueClicked(int mouseX, int mouseY);
    //bool isPauseExitToMenuClicked(int mouseX, int mouseY);
    int selectedButtonIndex;
private:
    sf::RenderWindow& window;
    
    sf::RectangleShape mainMenuStartButton;
    sf::RectangleShape mainMenuChangeColorButton;
    sf::RectangleShape mainMenuExitButton;

    sf::RectangleShape gameOverReplayButton;
    sf::RectangleShape gameOverToMainMenuButton;


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
};

#endif // GAMEWINDOWVIEW_HPP
