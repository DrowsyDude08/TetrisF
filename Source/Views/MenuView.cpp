#include "MenuView.hpp"

MenuView::MenuView(sf::RenderWindow& window)
    : window(window),
    palette(),
    hoveredButton(nullptr),
    selectedButtonIndex(0),
    currentState(GameState::MainMenu)
{
	std::cout << "create menuView beginning" << std::endl;
    initializeButtons();
    
    if (!font.loadFromFile(resourcePath + "/Fonts/ShoraiSansStdNVar.ttf")) {
        throw std::runtime_error("Failed to load Fonts for MenuView");

    }

    sf::Text::Style boldStyle = sf::Text::Bold;

    mainMenuStartText.setFont(font);
    mainMenuStartText.setString("Start");
    mainMenuStartText.setCharacterSize(24);
    mainMenuStartText.setFillColor(palette.defaultTextColor);
    mainMenuStartText.setStyle(boldStyle);
    mainMenuStartText.setPosition(
        (mainMenuStartButton.getPosition().x + (mainMenuStartButton.getSize().x - mainMenuStartText.getLocalBounds().width) / 2),
        (mainMenuStartButton.getPosition().y + (mainMenuStartButton.getSize().y - mainMenuStartText.getLocalBounds().height) / 2)
    );

    mainMenuChangeColorText.setFont(font);
    mainMenuChangeColorText.setString("Change Color");
    mainMenuChangeColorText.setCharacterSize(24);
    mainMenuChangeColorText.setFillColor(palette.defaultTextColor);
    mainMenuChangeColorText.setStyle(boldStyle);
    mainMenuChangeColorText.setPosition(
        (mainMenuChangeColorButton.getPosition().x + (mainMenuChangeColorButton.getSize().x - mainMenuChangeColorText.getLocalBounds().width) / 2),
        (mainMenuChangeColorButton.getPosition().y + (mainMenuChangeColorButton.getSize().y - mainMenuChangeColorText.getLocalBounds().height) / 2)
    );

    mainMenuExitText.setFont(font);
    mainMenuExitText.setString("Exit");
    mainMenuExitText.setCharacterSize(24);
    mainMenuExitText.setFillColor(palette.defaultTextColor);
    mainMenuExitText.setStyle(boldStyle);
    mainMenuExitText.setPosition(
        (mainMenuExitButton.getPosition().x + (mainMenuExitButton.getSize().x - mainMenuExitText.getLocalBounds().width) / 2),
        (mainMenuExitButton.getPosition().y + (mainMenuExitButton.getSize().y - mainMenuExitText.getLocalBounds().height) / 2)
    );

    gameOverReplayText.setFont(font);
    gameOverReplayText.setString("Restart");
    gameOverReplayText.setCharacterSize(24);
    gameOverReplayText.setFillColor(palette.defaultTextColor);
    gameOverReplayText.setPosition(300, 450);

    gameOverToMainMenuText.setFont(font);
    gameOverToMainMenuText.setString("To Main Menu");
    gameOverToMainMenuText.setCharacterSize(24);
    gameOverToMainMenuText.setFillColor(palette.defaultTextColor);
    gameOverToMainMenuText.setPosition(300, 500);

    selectMainMenuButton(selectedButtonIndex);

	std::cout << "create menuView end" << std::endl;
}



void MenuView::initializeButtons() {
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonSpacing = 20;

    float centerX = (window.getSize().x - buttonWidth) / 2;

    mainMenuStartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuStartButton.setPosition(centerX, 150);
    mainMenuStartButton.setFillColor(palette.defaultButtonColor);

    mainMenuChangeColorButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuChangeColorButton.setPosition(centerX, 150 + buttonHeight + buttonSpacing);
    mainMenuChangeColorButton.setFillColor(palette.defaultButtonColor);

    mainMenuExitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuExitButton.setPosition(centerX, 150 + 2 * (buttonHeight + buttonSpacing));
    mainMenuExitButton.setFillColor(palette.defaultButtonColor);

    gameOverReplayButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverReplayButton.setPosition(centerX, 450);
    gameOverReplayButton.setFillColor(palette.defaultButtonColor);

    gameOverToMainMenuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverToMainMenuButton.setPosition(centerX, 450 + buttonHeight + buttonSpacing);
    gameOverToMainMenuButton.setFillColor(palette.defaultButtonColor);
}


void MenuView::drawMainMenu() {
    window.clear(palette.background);
    window.draw(mainMenuStartButton);
    window.draw(mainMenuChangeColorButton);
    window.draw(mainMenuExitButton);
    window.draw(mainMenuStartText);
    window.draw(mainMenuChangeColorText);
    window.draw(mainMenuExitText);
}

void MenuView::handleMainMenuMouseMove(int mouseX, int mouseY, GameState gameState) {
    if (gameState != GameState::MainMenu) {
        return;
    }

    if (mainMenuStartButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuStartButton;
        selectedButtonIndex = 0;
    }
    else if (mainMenuChangeColorButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuChangeColorButton;
        selectedButtonIndex = 1;
    }
    else if (mainMenuExitButton.getGlobalBounds().contains(mouseX, mouseY)) {
        hoveredButton = &mainMenuExitButton;
        selectedButtonIndex = 2;
    }
    else {
        hoveredButton = nullptr;
    }

    updateMainMenuButtonAppearance();
}

void MenuView::handleMainMenuKeyboardInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedButtonIndex = (selectedButtonIndex + 2) % 3;
            selectMainMenuButton(selectedButtonIndex);
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedButtonIndex = (selectedButtonIndex + 1) % 3;
            selectMainMenuButton(selectedButtonIndex);
        }
        /*else if (event.type == sf::Keyboard::Enter) {
            selectMainMenuButton(selectedButtonIndex);
        }*/
    }
}

bool MenuView::isStartButtonClicked(int mouseX, int mouseY) {
    return mainMenuStartButton.getGlobalBounds().contains(mouseX, mouseY);
}

bool MenuView::isChangeColorButtonClicked(int mouseX, int mouseY) {
    return mainMenuChangeColorButton.getGlobalBounds().contains(mouseX, mouseY);
}

bool MenuView::isExitButtonClicked(int mouseX, int mouseY) {
    return mainMenuExitButton.getGlobalBounds().contains(mouseX, mouseY);
}

void MenuView::selectMainMenuButton(int index) {
    switch (index) {
    case 0:
        hoveredButton = &mainMenuStartButton;
        break;
    case 1:
        hoveredButton = &mainMenuChangeColorButton;
        break;
    case 2:
        hoveredButton = &mainMenuExitButton;
        break;
    default:
        hoveredButton = nullptr;
        break;
    }
    updateMainMenuButtonAppearance();
}

void MenuView::updateMainMenuButtonAppearance() {
    mainMenuStartButton.setFillColor(hoveredButton == &mainMenuStartButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    mainMenuChangeColorButton.setFillColor(hoveredButton == &mainMenuChangeColorButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    mainMenuExitButton.setFillColor(hoveredButton == &mainMenuExitButton ? palette.selectedButtonColor : palette.defaultButtonColor);

    mainMenuStartText.setFillColor(hoveredButton == &mainMenuStartButton ? palette.selectedTextColor : palette.defaultTextColor);
    mainMenuChangeColorText.setFillColor(hoveredButton == &mainMenuChangeColorButton ? palette.selectedTextColor : palette.defaultTextColor);
    mainMenuExitText.setFillColor(hoveredButton == &mainMenuExitButton ? palette.selectedTextColor : palette.defaultTextColor);
}

void MenuView::swapTextures() {
    std::swap(textureGhostPath, texturePiecePath);
}


void MenuView::drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore) {
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(palette.background.r, palette.background.g, palette.background.b, 150));  // Полупрозрачный фон
    window.draw(background);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(palette.defaultTextColor);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(
        (window.getSize().x - gameOverText.getLocalBounds().width) / 2,
        window.getSize().y * 0.2f);
    window.draw(gameOverText);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Your Score: " + std::to_string(currentScore));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(palette.defaultTextColor);
    scoreText.setPosition(
        (window.getSize().x - scoreText.getLocalBounds().width) / 2,
        window.getSize().y * 0.4f);
    window.draw(scoreText);

    if (!leaderboard.getTopScores().empty()) {
        sf::Text topScoreText;
        topScoreText.setFont(font);
        topScoreText.setString("Top Score: " + std::to_string(leaderboard.getTopScores().front().score));
        topScoreText.setCharacterSize(35);
        topScoreText.setFillColor(palette.defaultTextColor);
        topScoreText.setPosition(
            (window.getSize().x - topScoreText.getLocalBounds().width) / 2,
            window.getSize().y * 0.5f);
        window.draw(topScoreText);
    }

    sf::RectangleShape retryButton(sf::Vector2f(200, 50));
    retryButton.setFillColor(palette.defaultButtonColor);
    retryButton.setOutlineColor(palette.defaultOutlineColor);
    retryButton.setOutlineThickness(3);
    retryButton.setPosition((window.getSize().x - retryButton.getSize().x) / 2, window.getSize().y * 0.7f);
    window.draw(retryButton);

    sf::Text retryButtonText;
    retryButtonText.setFont(font);
    retryButtonText.setString("Retry");
    retryButtonText.setCharacterSize(30);
    retryButtonText.setFillColor(palette.defaultTextColor);
    retryButtonText.setPosition(
        retryButton.getPosition().x + (retryButton.getSize().x - retryButtonText.getLocalBounds().width) / 2,
        retryButton.getPosition().y + (retryButton.getSize().y - retryButtonText.getLocalBounds().height) / 2 - 5
    );
    window.draw(retryButtonText);

    sf::RectangleShape menuButton(sf::Vector2f(200, 50));
    menuButton.setFillColor(palette.defaultButtonColor);
    menuButton.setOutlineColor(palette.defaultOutlineColor);
    menuButton.setOutlineThickness(3);
    menuButton.setPosition((window.getSize().x - menuButton.getSize().x) / 2, window.getSize().y * 0.8f);
    window.draw(menuButton);

    sf::Text menuButtonText;
    menuButtonText.setFont(font);
    menuButtonText.setString("Main Menu");
    menuButtonText.setCharacterSize(30);
    menuButtonText.setFillColor(palette.defaultTextColor);
    menuButtonText.setPosition(
        menuButton.getPosition().x + (menuButton.getSize().x - menuButtonText.getLocalBounds().width) / 2,
        menuButton.getPosition().y + (menuButton.getSize().y - menuButtonText.getLocalBounds().height) / 2 - 5
    );
    window.draw(menuButtonText);
}
bool MenuView::isGameOverReplayClicked(int mouseX, int mouseY) {
    return gameOverReplayButton.getGlobalBounds().contains(mouseX, mouseY);
}
bool MenuView::isGameOverToMainMenuClicked(int mouseX, int mouseY) {
    return gameOverToMainMenuButton.getGlobalBounds().contains(mouseX, mouseY);
}
void MenuView::selectGameOverButton(int index) {
    switch (index) {
    case 0:
        hoveredButton = &gameOverReplayButton;
        break;
    case 1:
        hoveredButton = &gameOverToMainMenuButton;
        break;
    default:
        hoveredButton = nullptr;
        break;
    }
    updateGameOverButtonAppearance();
}
/**
* Обновляет внешний вид кнопок меню game over в зависимости от того, на какую кнопку в данный момент наведен курсор.
*
* @param Кнопка наведения курсора - указатель на кнопку, на которую в данный момент наведен курсор. Если кнопка не наведена, это должно быть значение nullptr.
* * @return void
*/
void MenuView::updateGameOverButtonAppearance() {
    gameOverReplayButton.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    gameOverToMainMenuButton.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedButtonColor : palette.defaultButtonColor);

    gameOverReplayText.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedTextColor : palette.defaultTextColor);
    gameOverToMainMenuText.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedTextColor : palette.defaultTextColor);
}

void MenuView::handleMainMenuEvent(const sf::Event& event) {
    if (currentState == GameState::MainMenu) {
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                switch ( selectedButtonIndex) {
                case 0:
                    currentState = GameState::InGame;
                    break;
                case 1:
                    swapTextures();
                    break;
                case 2:
                    exit(0);
                }
            }
            else if (isStartButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                currentState = GameState::InGame;
            }
            else if (isChangeColorButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                swapTextures();
            }
            else if (isExitButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                exit(0);
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            handleMainMenuMouseMove(event.mouseMove.x, event.mouseMove.y, currentState);
        }
        else if (event.type == sf::Event::KeyPressed) {
            handleMainMenuKeyboardInput(event);
        }
    }
    else if (currentState == GameState::GameOver) {
        if (isGameOverReplayClicked(event.mouseButton.x, event.mouseButton.y)) {
            currentState = GameState::InGame;
        }
        else if ( isGameOverToMainMenuClicked(event.mouseButton.x, event.mouseButton.y)) {
            currentState = GameState::MainMenu;
        }
    }
}