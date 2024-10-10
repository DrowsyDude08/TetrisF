#include "GameView.hpp"

GameView::GameView(sf::RenderWindow& window)
    : lockDelay(0.5),
    lockDelayValue(lockDelay),
    lockCount(14),
    lockCountValue(lockCount),
    totalLineCount(0),
    totalPieceCount(0),
    boardWobble(0),
    pieceIndicatorShapeAlpha(0),
    lineClearCombo(0),
    btbCombo(0),
    elapsedTime(trackingFrameTime.getElapsedTime().asSeconds()),
    selectedButtonIndex(-1)
{
    board[HEIGHT][WIDTH] = { 0 };
};

void GameView::drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore){}

void GameView::handleInput(const sf::Event& event){}
void GameView::handleGameOverMouseMove(int mouseX, int mouseY){}
void GameView::handleGameOverKeyboardInput(const sf::Event& event){}

void GameView::initializeTextures()
{
    textureGhost.loadFromFile(textureGhostPath);
    spriteGhost.getTexture(textureGhost);
    spriteGhost.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));

    sf::Texture texturePiece;
    texturePiece.loadFromFile(texturePiecePath);
    sf::Sprite spritePiece(texturePiece);
    spritePiece.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));

    sf::Texture textureLockedPiece;
    textureLockedPiece.loadFromFile(resourcePath + "/Sprites/piece_lock.png");
    sf::Sprite spriteLockedPiece(textureLockedPiece);
    spriteLockedPiece.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));

    sf::Texture textureBoard;
    textureBoard.loadFromFile(resourcePath + "/Sprites/board.png");
    sf::Sprite spriteBoard(textureBoard);

    sf::Font fontGame;
    fontGame.loadFromFile(resourcePath + "/Fonts/Minecraft.ttf");
}

bool GameView::isGameOverReplayClicked(int mouseX, int mouseY){ 
        return gameOverReplayButton.getGlobalBounds().contains(mouseX, mouseY); 
    }
bool GameView::isGameOverToMainMenuClicked(int mouseX, int mouseY) {
        return gameOverToMainMenuButton.getGlobalBounds().contains(mouseX, mouseY);
    }

    //---------------
void GameView::drawBackground(){
        sf::RectangleShape backboardShape;
        backboardShape.setSize(sf::Vector2f(320, 650));
        backboardShape.setFillColor(sf::Color::White);
        backboardShape.setPosition(140, 20 + boardWobble);
        gameWindow.draw(backboardShape);

        backboardShape.setSize(sf::Vector2f(320, 20));
        backboardShape.setFillColor(sf::Color::Black);
        backboardShape.setPosition(140, 0 + boardWobble);
        gameWindow.draw(backboardShape);

        backboardShape.setSize(sf::Vector2f(320, 10));
        backboardShape.setFillColor(sf::Color::White);
        backboardShape.setPosition(140, 10 + boardWobble);
        gameWindow.draw(backboardShape);

    }
void GameView::drawPlayingField(){
        spriteBoard.setPosition(150, -30 + boardWobble);
        gameWindow.draw(spriteBoard);
    }
void GameView::drawDAS(){
        float dasProgress = std::max((double)0, (double)(lockDelayValue / lockDelay));

        sf::RectangleShape dasBarShape;
        dasBarShape.setSize(sf::Vector2f(dasProgress * 320, 8));
        dasBarShape.setFillColor(sf::Color::White);
        dasBarShape.setPosition(140, 700 + boardWobble);
        gameWindow.draw(dasBarShape);
    }
void GameView::drawHeldTetramino(){
        sf::CircleShape lockCountCircle;
        lockCountCircle.setRadius(6);
        lockCountCircle.setFillColor(sf::Color::White);
        for (int i = 0; i < lockCountValue; i++) {
            lockCountCircle.setPosition(141 + (i * 23.5), 680 + boardWobble);
            gameWindow.draw(lockCountCircle);
        }

        spritePiece.setColor(sf::Color(255, 255, 255, 255));
        if (holdPiece != -1) {
            Piece holdPieceBlocks[4];
            int holdPieceIndex = holdPiece;

            for (int i = 0; i < 4; i++) {
                holdPieceBlocks[i].x = pieces[holdPieceIndex][i] % 2 + 4;
                holdPieceBlocks[i].y = pieces[holdPieceIndex][i] / 2 + 3;

                if (holdPiece == J_TETROMINO) {
                    holdPieceBlocks[i].x--;
                }
            }

            for (int i = 0; i < 4; i++) {
                Piece center = holdPieceBlocks[1];
                if (holdPiece == I_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, 1);
                }
                if (holdPiece == T_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, -1);
                }
                if (holdPiece == S_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, -1);
                }
                if (holdPiece == Z_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, -1);
                }
                if (holdPiece == L_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, -1);
                }
                if (holdPiece == J_TETROMINO) {
                    holdPieceBlocks[i].rotate(center, 1);
                }
            }

            for (int i = 0; i < 4; i++) {
                spritePiece.setTextureRect(sf::IntRect(holdPiece * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
                int xOffset = 0;
                if (holdPiece == I_TETROMINO || holdPiece == O_TETROMINO) {
                    xOffset = 15;
                }
                spritePiece.setPosition(holdPieceBlocks[i].x * TEXTURE_SIZE - 65 - xOffset, holdPieceBlocks[i].y * TEXTURE_SIZE - 10);
                gameWindow.draw(spritePiece);
            }
        }
    }
void GameView::drawNextTetraminos(){
        spritePiece.setColor(sf::Color(255, 255, 255, 255));
        for (int i = 0; i < bagOfSevenPieces.size() + nextBagOfSevenPieces.size(); i++) {
            if (i < 5) {
                Piece nextPiece[4];
                int nextPieceChoose;
                if (i < bagOfSevenPieces.size()) {
                    nextPieceChoose = bagOfSevenPieces.at(i);
                }
                else {
                    nextPieceChoose = nextBagOfSevenPieces.at(i - bagOfSevenPieces.size());
                }

                int nextColor = nextPieceChoose;
                for (int j = 0; j < 4; j++) {
                    nextPiece[j].x = pieces[nextPieceChoose][j] % 2 + 4;
                    nextPiece[j].y = pieces[nextPieceChoose][j] / 2 + 3;

                    if (nextColor == J_TETROMINO) {
                        nextPiece[j].x--;
                    }
                }

                for (int j = 0; j < 4; j++) {
                    Piece center = nextPiece[1];
                    if (nextColor == I_TETROMINO) {
                        nextPiece[j].rotate(center, 1);
                    }
                    if (nextColor == T_TETROMINO) {
                        nextPiece[j].rotate(center, -1);
                    }
                    if (nextColor == S_TETROMINO) {
                        nextPiece[j].rotate(center, -1);
                    }
                    if (nextColor == Z_TETROMINO) {
                        nextPiece[j].rotate(center, -1);
                    }
                    if (nextColor == L_TETROMINO) {
                        nextPiece[j].rotate(center, -1);
                    }
                    if (nextColor == J_TETROMINO) {
                        nextPiece[j].rotate(center, 1);
                    }
                }

                for (int j = 0; j < 4; j++) {
                    spritePiece.setTextureRect(sf::IntRect(nextColor * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
                    int xOffset = 0;
                    if (nextColor == I_TETROMINO || nextColor == O_TETROMINO) {
                        xOffset = 15;
                    }
                    spritePiece.setPosition(nextPiece[j].x * TEXTURE_SIZE + 395 - xOffset, nextPiece[j].y * TEXTURE_SIZE - 10 + (90 * i));
                    gameWindow.draw(spritePiece);
                }
            }
        }
    }
void GameView::drawFieldTetramino(){
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                spritePiece.setColor(sf::Color(255, 255, 255, 255));
                if (board[i][j] == 0) {
                    continue;
                }
                spritePiece.setTextureRect(sf::IntRect((board[i][j] - 1) * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
                spritePiece.setPosition(j * TEXTURE_SIZE + 150, i * TEXTURE_SIZE - 90 + boardWobble);
                gameWindow.draw(spritePiece);
            }
        }
    }
void GameView::drawLockedTetramino(){
        for (int i = 0; i < piecesLock.size(); i++) {
            for (int j = 0; j < 4; j++) {
                spriteLockedPiece.setTextureRect(sf::IntRect((int)piecesLock.at(i).at(j).animation * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
                spriteLockedPiece.setColor(sf::Color(255, 255, 255, 200));
                spriteLockedPiece.setPosition(piecesLock.at(i).at(j).x * TEXTURE_SIZE + 150, (piecesLock.at(i).at(j).y - 1) * TEXTURE_SIZE - 90 + boardWobble);
                gameWindow.draw(spriteLockedPiece);

                piecesLock.at(i).at(j).animation += elapsedTime * FRAMERATE * 4;
            }

            for (int j = 0; j < 4; j++) {
                if (piecesLock.at(i).at(j).animation >= 67) {
                    piecesLock.erase(piecesLock.begin() + i);
                    break;
                }
            }
        }
    }
void GameView::drawGhostTetramino(){
        spriteGhost.setTextureRect(sf::IntRect(colorPiece * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
        for (int i = 0; i < 4; i++) {
            spriteGhost.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, ghost[i].y * TEXTURE_SIZE - 90 + boardWobble);
            gameWindow.draw(spriteGhost);
        }
    }
void GameView::drawCurrentTetramino(){
        pieceIndicatorShapeAlpha = (sin(elapsedGameTime.getElapsedTime().asSeconds() * 10) + 1) * 30;
        sf::RectangleShape pieceIndicatorShape;
        pieceIndicatorShape.setSize(sf::Vector2f(30, 30));
        pieceIndicatorShape.setFillColor(sf::Color(255, 255, 255, pieceIndicatorShapeAlpha));

        double pieceAlpha = std::max((double)0, (double)(lockDelayValue / lockDelay));
        for (int i = 0; i < 4; i++) {
            spritePiece.setTextureRect(sf::IntRect(7 * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
            spritePiece.setColor(sf::Color(255, 255, 255, 255));
            spritePiece.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, currentPiece[i].y * TEXTURE_SIZE - 90 + boardWobble);
            gameWindow.draw(spritePiece);

            spritePiece.setTextureRect(sf::IntRect(colorPiece * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
            spritePiece.setColor(sf::Color(255, 255, 255, pieceAlpha * 255));
            spritePiece.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, currentPiece[i].y * TEXTURE_SIZE - 90 + boardWobble);
            gameWindow.draw(spritePiece);

            pieceIndicatorShape.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, currentPiece[i].y * TEXTURE_SIZE - 90 + boardWobble);
            gameWindow.draw(pieceIndicatorShape);
        }
    }
void GameView::drawParticles(){
        for (int i = 0; i < particles.size(); i++) {
            particles.at(i).update(elapsedTime);
            particles.at(i).draw(&gameWindow);
            if (particles.at(i).alpha <= 0) {
                particles.erase(particles.begin() + i);
            }
        }
    }
void GameView::drawText(){
        sf::Text text;
        text.setFont(fontGame);
        text.setCharacterSize(35);
        text.setFillColor(sf::Color::White);

        text.setString("NEXT");
        text.setPosition(480, 30);
        gameWindow.draw(text);

        text.setString("HOLD");
        text.setPosition(25, 30);
        gameWindow.draw(text);

        text.setCharacterSize(22);
        text.setString(std::to_string(lineClearCombo - 1) + " COMBO");
        text.setPosition(475, 650);
        if (lineClearCombo - 1 > 0) gameWindow.draw(text);

        sf::RectangleShape textBox;
        sf::Rect textRectangle = text.getLocalBounds();;
        textBox.setFillColor(sf::Color::White);

        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(25);
        text.setString("LINES");
        textRectangle = text.getLocalBounds();
        text.setPosition(120 - textRectangle.width, 400 + boardWobble);
        textBox.setSize(sf::Vector2f(textRectangle.width + 40, textRectangle.height * 2 - 4));
        textBox.setOrigin(sf::Vector2f(20, 0));
        textBox.setPosition(text.getPosition());
        gameWindow.draw(textBox);
        gameWindow.draw(text);

        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);
        text.setString(std::to_string(totalLineCount));
        textRectangle = text.getLocalBounds();
        text.setPosition(120 - textRectangle.width, 450 + boardWobble);
        gameWindow.draw(text);

        text.setFillColor(sf::Color::Black);
        text.setCharacterSize(25);
        text.setString("PIECES");
        textRectangle = text.getLocalBounds();
        text.setPosition(120 - textRectangle.width, 510 + boardWobble);
        textBox.setSize(sf::Vector2f(textRectangle.width + 40, textRectangle.height * 2 - 4));
        textBox.setOrigin(sf::Vector2f(20, 0));
        textBox.setPosition(text.getPosition());
        gameWindow.draw(textBox);
        gameWindow.draw(text);

        text.setFillColor(sf::Color::White);
        text.setCharacterSize(30);
        text.setString(std::to_string(totalPieceCount));
        textRectangle = text.getLocalBounds();
        text.setPosition(120 - textRectangle.width, 560 + boardWobble);
        gameWindow.draw(text);


}
    //---------------

void GameView::initializeButtons(){}
void GameView::selectGameOverButton(int index){}
void GameView::updateGameOverButtonAppearance(){}


