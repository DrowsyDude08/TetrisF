#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cmath>
#include "../Models/Tetramino.hpp"
#include "../Views/GameView.hpp"
#include "../Views/MenuVIew.hpp"
#include "../Utils/LeaderBoard.hpp"
#include "../Utils/GameStates.hpp"
#include "../Utils/Piece.hpp"


class GameController {
public:
    GameController();

    void runGame();




protected:
    sf::RenderWindow window;
    LeaderBoard leaderboard;
    GameView gameView;
    MenuView menuView;
    GameState currentState;
    

    bool isGameOver;

    std::vector<int> generateNewBag();
    void startTetrisGame();
    bool isCollided();
    bool isCollidedGhost();
    bool isDead();
    int sign(int num); 
    int calculateScore(int lineClearCount, int combo);

    //----------------------------
    int board[HEIGHT][WIDTH] = { 0 };

    int pieces[7][4] = {
    {3, 4, 5, 6},   ///< Z-образное тетромино
    {2, 4, 6, 7},   ///< L-образное тетромино
    {2, 3, 4, 5},   ///< O-образное тетромино (квадрат)
    {2, 4, 5, 7},   ///< S-образное тетромино
    {1, 3, 5, 7},   ///< I-образное тетромино (линия)
    {3, 5, 6, 7},   ///< J-образное тетромино
    {2, 4, 5, 6}    ///< T-образное тетромино
    };

    //----------------------------

    int TEXTURE_SIZE = 30;

    // Флаги управления
    int isRotateClockWise;
    int isRotateCounterClockwise;
    int isHardDrop;
    int isMoveRight;
    int isMoveLeft;
    int isHoldPiece;
    int isSidehit;
    int isTSpin;
    int isRestart;
    int score;

    int keyRotateClockwise;
    int keyRotateCounterClockwise;
    int keyHardDrop;
    int keyMoveRight;
    int keyMoveLeft;
    int keyRightHold;
    int keyLeftHold;
    bool isOnFocus;
    bool isTouchGround;
    int lastKeyPressed;
    int keyRestart;

    double gameTimer;
    double gameDelay;
    double fastDropDelay;
    double airSoftDrop;
    double airSoftDropValue;

    Piece currentPiece[4]{}, previousPiecePosition[4]{}, ghost[4]{};
};

#endif // GAMECONTROLLER_HPP