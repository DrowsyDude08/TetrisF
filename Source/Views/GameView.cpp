#include "GameView.hpp"

GameView::GameView(sf::RenderWindow& window)
    : holdPiece(0),
    moveX(0),
    rotatePiece(0),
    colorPiece(0),
    hardDrop(0),
    holded(0),
    moveLeft(0),
    moveRight(0),
    start(0),
    lockDelay(0),
    lockDelayValue(0.5),
    lockCount(0),
    lockCountValue(15),
    totalLineCount(0),
    totalPieceCount(0),
    boardWobble(0),
    pieceIndicatorShapeAlpha(255),
    lineClearCombo(0),
    btbCombo(0),
    elapsedTime(0.0f),
    selectedButtonIndex(-1),

    hoveredButton(nullptr) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
    window.setFramerateLimit(FRAMERATE);
    
    trackingFrameTime.restart();
    trackingGameTime.restart();
    elapsedGameTime.restart();

    initializeTextures();
    fontGame.loadFromFile(resourcePath + "/Fonts/Minecraft.ttf");

    initializeButtons(window);
}

void GameView::drawGameOverMenu(const LeaderBoard& leaderboard, int currentScore){}

void GameView::handleInput(const sf::Event& event){}
void GameView::handleGameOverMouseMove(int mouseX, int mouseY){}
void GameView::handleGameOverKeyboardInput(const sf::Event& event){}

void GameView::initializeTextures() {
    if (!textureGhost.loadFromFile(textureGhostPath)) {
        throw std::runtime_error("Failed to load ghost texture");
    }
    spriteGhost.setTexture(textureGhost);
    spriteGhost.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));
    if (!texturePiece.loadFromFile(texturePiecePath)) {
        throw std::runtime_error("Failed to load piece texture");
    }
    spritePiece.setTexture(texturePiece);
    spritePiece.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));

    if (!textureLockedPiece.loadFromFile(resourcePath + "/Sprites/piece_lock.png")) {
        throw std::runtime_error("Failed to load locked piece texture");
    }
    spriteLockedPiece.setTexture(textureLockedPiece);
    spriteLockedPiece.setTextureRect(sf::IntRect(0, 0, TEXTURE_SIZE, TEXTURE_SIZE));

    if (!textureBoard.loadFromFile(resourcePath + "/Sprites/board.png")) {
        throw std::runtime_error("Failed to load board texture");
    }
    spriteBoard.setTexture(textureBoard);

    if (!fontGame.loadFromFile(resourcePath + "/Fonts/Minecraft.ttf")) {
        throw std::runtime_error("Failed to load font");
    }
}

bool GameView::isGameOverReplayClicked(int mouseX, int mouseY){ 
        return gameOverReplayButton.getGlobalBounds().contains(mouseX, mouseY); 
    }
bool GameView::isGameOverToMainMenuClicked(int mouseX, int mouseY) {
        return gameOverToMainMenuButton.getGlobalBounds().contains(mouseX, mouseY);
    }

    //---------------
void GameView::drawBackground(sf::RenderWindow& gameWindow){
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
void GameView::drawPlayingField(sf::RenderWindow& gameWindow){
        spriteBoard.setPosition(150, -30 + boardWobble);
        gameWindow.draw(spriteBoard);
    }
void GameView::drawDAS(sf::RenderWindow& gameWindow){
        float dasProgress = std::max((double)0, (double)(lockDelayValue / lockDelay));

        sf::RectangleShape dasBarShape;
        dasBarShape.setSize(sf::Vector2f(dasProgress * 320, 8));
        dasBarShape.setFillColor(sf::Color::White);
        dasBarShape.setPosition(140, 700 + boardWobble);
        gameWindow.draw(dasBarShape);
    }
void GameView::drawHeldTetramino(sf::RenderWindow& gameWindow){
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
void GameView::drawNextTetraminos(sf::RenderWindow& gameWindow){
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
void GameView::drawFieldTetramino(sf::RenderWindow& gameWindow){
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
void GameView::drawLockedTetramino(sf::RenderWindow& gameWindow){
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
void GameView::drawGhostTetramino(sf::RenderWindow& gameWindow){
        spriteGhost.setTextureRect(sf::IntRect(colorPiece * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
        for (int i = 0; i < 4; i++) {
            spriteGhost.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, ghost[i].y * TEXTURE_SIZE - 90 + boardWobble);
            gameWindow.draw(spriteGhost);
        }
    }
void GameView::drawCurrentTetramino(sf::RenderWindow& gameWindow){
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
void GameView::drawParticles(sf::RenderWindow& gameWindow){
        for (int i = 0; i < particles.size(); i++) {
            particles.at(i).update(elapsedTime);
            particles.at(i).draw(&gameWindow);
            if (particles.at(i).alpha <= 0) {
                particles.erase(particles.begin() + i);
            }
        }
    }
void GameView::drawText(sf::RenderWindow& gameWindow){
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
void GameView::createParticle(std::vector<Particle>* particles) {
    for (int i = 0; i < 4; i++) {
        Particle particle((currentPiece[i].x * 30) + 150 + 15 + (rand() % 60 - 30),
            (currentPiece[i].y * 30) - 60 - 30,
            rand() % 250 + 150,
            rand() % 360);
        particles->push_back(particle);
    }
}
void GameView::initializeButtons(sf::RenderWindow& gameWindow){
    float buttonWidth = 200;
    float buttonHeight = 50;
    float buttonSpacing = 20;

    float centerX = (gameWindow.getSize().x - buttonWidth) / 2;


    gameOverReplayButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverReplayButton.setPosition(centerX, 450);
    gameOverReplayButton.setFillColor(palette.defaultButtonColor);

    gameOverToMainMenuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    gameOverToMainMenuButton.setPosition(centerX, 450 + buttonHeight + buttonSpacing);
    gameOverToMainMenuButton.setFillColor(palette.defaultButtonColor);
}
void GameView::selectGameOverButton(int index){
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
void GameView::updateGameOverButtonAppearance(){
    gameOverReplayButton.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedButtonColor : palette.defaultButtonColor);
    gameOverToMainMenuButton.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedButtonColor : palette.defaultButtonColor);

    gameOverReplayText.setFillColor(hoveredButton == &gameOverReplayButton ? palette.selectedTextColor : palette.defaultTextColor);
    gameOverToMainMenuText.setFillColor(hoveredButton == &gameOverToMainMenuButton ? palette.selectedTextColor : palette.defaultTextColor);
}

