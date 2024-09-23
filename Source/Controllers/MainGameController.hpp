#ifndef MAINGAMECONTROLLER_HPP
#define MAINGAMECONTROLLER_HPP

#include "../Views/GameWindowView.hpp"
#include "../Utils/LeaderBoard.hpp"
#include "../Utils/GameStates.hpp"
#include "../Utils/Piece.hpp"
#include "../Utils/Particle.hpp"
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <thread>
#include <vector>

constexpr int WIDTH = 10;
constexpr int HEIGHT = 25;
constexpr int FRAMERATE = 75;

constexpr auto Z_TETROMINO = 0;
constexpr auto L_TETROMINO = 1;
constexpr auto O_TETROMINO = 2;
constexpr auto S_TETROMINO = 3;
constexpr auto I_TETROMINO = 4;
constexpr auto J_TETROMINO = 5;
constexpr auto T_TETROMINO = 6;

class MainGameController {
public:
    MainGameController();

    void runGame();

protected:
    sf::RenderWindow window;
    LeaderBoard leaderboard;
    GameWindowView gameWindowView;
    GameState currentState = GameState::MainMenu;
   
    int score = 0;
    bool isGameOver = false;
    std::string resourcePath = RESOURCE_DIR;
    int board[HEIGHT][WIDTH] = { 0 };

    Piece currentPiece[4]{}, previousPiecePosition[4]{}, ghost[4]{};
    std::vector<std::vector<PieceLock>> piecesLock;
    std::vector<Particle> particles;
    int pieces[7][4] = {
    {3, 4, 5, 6},   ///< Z-образное тетромино
    {2, 4, 6, 7},   ///< L-образное тетромино
    {2, 3, 4, 5},   ///< O-образное тетромино (квадрат)
    {2, 4, 5, 7},   ///< S-образное тетромино
    {1, 3, 5, 7},   ///< I-образное тетромино (линия)
    {3, 5, 6, 7},   ///< J-образное тетромино
    {2, 4, 5, 6}    ///< T-образное тетромино
    };

    std::vector<int> generateNewBag();
    void handleMainMenuEvent(const sf::Event& event);
    void startTetrisGame();
    bool isCollided();
    bool isCollidedGhost();
    bool isDead();
    void createParticle(std::vector<Particle>* particles);
    int sign(int num);
    int calculateScore(int lineClearCount, int combo);

    void swapTextures();

    std::string texturePiecePath = resourcePath + "/Sprites/jstris1.png";
    std::string textureGhostPath = resourcePath + "/Sprites/ghost.png";
    
    int textureSize = 30;
};

#endif // MAINGAMECONTROLLER_HPP
