#include "GameController.hpp"


    std::vector<int> generateNewBag(){
        std::vector<int> templateBag;

        for (int i = 0; i < 7; i++) {
            templateBag.push_back(i);
        }

        std::vector<int> newBag;

        while (templateBag.size() > 0) {
            int index = rand() % templateBag.size();
            int choosePiece = templateBag.at(index);
            templateBag.erase(templateBag.begin() + index);
            newBag.push_back(choosePiece);
        }

        return newBag;
    }

    bool isCollided(){
        for (int i = 0; i < 4; i++) {
            if (currentPiece[i].x < 0 || currentPiece[i].x >= WIDTH || currentPiece[i].y >= HEIGHT) {
                return false;
            }
            else if (board[currentPiece[i].y][currentPiece[i].x]) {
                return false;
            }
        }
        return true;
    }
    bool isCollidedGhost(){
        for (int i = 0; i < 4; i++) {
            if (ghost[i].y >= HEIGHT) {
                return false;
            }
            else if (board[ghost[i].y][ghost[i].x]) {
                return false;
            }
        }
        return true;
    }
    bool isDead(){
        for (int i = 0; i < 4; i++) {
            if (board[currentPiece[i].y][currentPiece[i].x]) {
                return true;
            }
        }
        return false;
    }
    int sign(int num){
        if (num < 0) return -1;
        else if (num > 0) return 1;
        else return 0;
    }
    int calculateScore(int lineClearCount, int combo){
        int points = 0;
        switch (lineClearCount) {
        case 1: points = 100; break;
        case 2: points = 300; break;
        case 3: points = 500; break;
        case 4: points = 800; break;
        }
        points *= (combo + 1);
        return points;
    }

    void startTetrisGame() {}