#ifndef GAMEVIEW_HPP
#define GAMEVIEW_HPP

#include <SFML/Graphics.hpp>
#include "../Utils/ColorPalette.hpp"
#include "../Utils/LeaderBoard.hpp"
#include "../Utils/GameStates.hpp"
#include <cmath>
#include "../Utils/Piece.hpp"
#include "../Models/Tetramino.hpp"
#include "../Utils/Particle.hpp"

constexpr int TEXTURE_SIZE = 30;
constexpr int WIDTH = 10;
constexpr int HEIGHT = 25;
constexpr int FRAMERATE = 75;

class GameView {
public:
    GameView(sf::RenderWindow& window);
    sf::RenderWindow gameWindow;

    void drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore);
    

    void handleInput(const sf::Event& event);
    void handleGameOverMouseMove(int mouseX, int mouseY);
    void handleGameOverKeyboardInput(const sf::Event& event);
    void initializeTextures();
    bool isGameOverReplayClicked(int mouseX, int mouseY);
    bool isGameOverToMainMenuClicked(int mouseX, int mouseY);
    int selectedButtonIndex;

protected:

    int holdPiece;
    int moveX;
    int rotatePiece;
    int colorPiece;
    int hardDrop;
    int holded;
    int moveLeft;
    int moveRight;
    int start;
    double lockDelay;
    double lockDelayValue;
    int lockCount;
    int lockCountValue;
    int totalLineCount;
    int totalPieceCount;
    double boardWobble;
    double pieceIndicatorShapeAlpha;
    int lineClearCombo;
    int btbCombo;
    int board[HEIGHT][WIDTH];
    float elapsedTime;
    std::vector<int> bagOfSevenPieces;
    std::vector<int> nextBagOfSevenPieces;
    sf::Clock trackingFrameTime;
    sf::Clock trackingGameTime;
    sf::Clock elapsedGameTime;
    int pieces[7][4] = {
    {3, 4, 5, 6},   ///< Z-образное тетромино
    {2, 4, 6, 7},   ///< L-образное тетромино
    {2, 3, 4, 5},   ///< O-образное тетромино (квадрат)
    {2, 4, 5, 7},   ///< S-образное тетромино
    {1, 3, 5, 7},   ///< I-образное тетромино (линия)
    {3, 5, 6, 7},   ///< J-образное тетромино
    {2, 4, 5, 6}    ///< T-образное тетромино
    };

    sf::Texture textureGhost;
    sf::Sprite spriteGhost;
    sf::Texture texturePiece;
    sf::Sprite spritePiece;
    sf::Texture textureLockedPiece;
    sf::Sprite spriteLockedPiece;
    sf::Texture textureBoard;
    sf::Sprite spriteBoard;
    sf::Font fontGame;


    Piece currentPiece[4]{}, previousPiecePosition[4]{}, ghost[4]{};
    std::vector<std::vector<PieceLock>> piecesLock;
    std::vector<Particle> particles;

    sf::RectangleShape gameOverReplayButton;
    sf::RectangleShape gameOverToMainMenuButton;
    sf::RectangleShape* hoveredButton;

    sf::Font font;
    sf::Text gameOverReplayText;
    sf::Text gameOverToMainMenuText;
    ColorPalette palette;
    //---------------
    void drawBackground();
    void drawPlayingField();
    void drawDAS();
    void drawHeldTetramino();
    void drawNextTetraminos();
    void drawFieldTetramino();
    void drawLockedTetramino();
    void drawGhostTetramino();
    void drawCurrentTetramino();
    void drawParticles();
    void drawText();
    //---------------
    std::string resourcePath = RESOURCE_DIR;

    void initializeButtons();
    void selectGameOverButton(int index);
    void updateGameOverButtonAppearance();

    std::string texturePiecePath = resourcePath + "/Sprites/jstris1.png";
    std::string textureGhostPath = resourcePath + "/Sprites/ghost.png";

};

#endif // GAMEWINDOWVIEW_HPP
