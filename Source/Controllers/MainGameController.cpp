#include "MainGameController.hpp"

#include "MainGameController.hpp"

MainGameController::MainGameController()
    : window(sf::VideoMode(600, 720), "Tetris Game"), 
    gameWindowView(window),
    leaderboard(),
    isGameOver(false)
{
    window.setFramerateLimit(FRAMERATE);

}
void MainGameController::runGame() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                handleMainMenuEvent(event);
            }
        }

        window.clear();

        switch (currentState) {
        case GameState::MainMenu:
            gameWindowView.drawMainMenu();
            break;
        case GameState::InGame:
            startTetrisGame();
            break;
        case GameState::GameOver:
            gameWindowView.drawGameOverMenu(leaderboard, score);
            leaderboard.addScore("Игрок 1", score);
            break;
        }

        window.display();
    }
}

void MainGameController::handleMainMenuEvent(const sf::Event& event) {
    if (currentState == GameState::MainMenu) {
        if (event.type == sf::Event::MouseButtonPressed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                switch (gameWindowView.selectedButtonIndex) {
                case 0:
                    currentState = GameState::InGame;
                    break;
                case 1:
                    swapTextures();
                    break;
                case 2: 
                    std::cout << "Exit\n";
                    exit(0);
                }
            }
            else if (gameWindowView.isStartButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                currentState = GameState::InGame;
            }
            else if (gameWindowView.isChangeColorButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                swapTextures(); 
            }
            else if (gameWindowView.isExitButtonClicked(event.mouseButton.x, event.mouseButton.y)) {
                std::cout << "Exit\n";
                exit(0);
            }
        }
        else if (event.type == sf::Event::MouseMoved) {
            gameWindowView.handleMainMenuMouseMove(event.mouseMove.x, event.mouseMove.y, currentState);
        }
        else if (event.type == sf::Event::KeyPressed) {
            gameWindowView.handleMainMenuKeyboardInput(event);
        }
    }
    else if (currentState == GameState::GameOver) {
        if (gameWindowView.isGameOverReplayClicked(event.mouseButton.x, event.mouseButton.y)) {
            currentState = GameState::InGame;
        }
        else if (gameWindowView.isGameOverToMainMenuClicked(event.mouseButton.x, event.mouseButton.y)) {
            currentState = GameState::MainMenu;
        }
    }
}

void MainGameController::swapTextures() {
    std::swap(textureGhostPath, texturePiecePath);
    std::cout << "Textures swapped!\n";
}
std::vector<int> MainGameController::generateNewBag() {
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

bool MainGameController::isCollided() {
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

bool MainGameController::isCollidedGhost() {
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

bool MainGameController::isDead() {
    for (int i = 0; i < 4; i++) {
        if (board[currentPiece[i].y][currentPiece[i].x]) {
            return true;
        }
    }
    return false;
}

void MainGameController::createParticle(std::vector<Particle>* particles) {
    for (int i = 0; i < 4; i++) {
        Particle particle((currentPiece[i].x * 30) + 150 + 15 + (rand() % 60 - 30),
            (currentPiece[i].y * 30) - 60 - 30,
            rand() % 250 + 150,
            rand() % 360);
        particles->push_back(particle);
    }
}

int MainGameController::sign(int num) {
    if (num < 0) return -1;
    else if (num > 0) return 1;
    else return 0;
}

int MainGameController::calculateScore(int lineClearCount, int combo) {
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
void MainGameController::startTetrisGame() {
    //в Конструктор GameView
    sf::RenderWindow gameWindow(sf::VideoMode(600, 720), "Tetris");
    srand(static_cast<unsigned int>(time(0))); 
    
    sf::Texture textureGhost;
    textureGhost.loadFromFile(textureGhostPath);
    sf::Sprite spriteGhost(textureGhost);
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

    int holdPiece;
    int moveX; 
    int rotatePiece;
    int colorPiece; 
    int hardDrop;
    int holded; 
    int moveLeft; 
    int moveRight;   
    int start;       

    sf::Clock trackingFrameTime;         
    sf::Clock trackingGameTime;       
    sf::Clock elapsedGameTime;  

    double gameTimer;                  
    double gameDelay;             
    std::vector<int> bagOfSevenPieces;   
    std::vector<int> nextBagOfSevenPieces; 

restart:
        double airSoftDrop = 8;
        double lockDelay = 0.5;
        double fastDropDelay = 0;
        int lockCount = 14;
        bool isTouchGround = false;

        int totalLineCount = 0;
        int totalPieceCount = 0;

        int lockCountValue = lockCount;
        double airSoftDropValue = airSoftDrop;
        double lockDelayValue = lockDelay;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                board[i][j] = 0;
            }
        }

        start = 1;
        gameTimer = 0;
        gameDelay = 0.5;
        moveX = 0;
        colorPiece = 0;
        hardDrop = 0;
        rotatePiece = 0;
        holdPiece = -1;
        moveLeft = 0;
        moveRight = 0;
        holded = 0;
        bagOfSevenPieces = generateNewBag();
        nextBagOfSevenPieces = generateNewBag();

        double pieceIndicatorShapeAlpha = 0;
        double boardWobble = 0;

        int lineClearCombo = 0;
        int btbCombo = 0;

        // Выбор первого тетромино
        int chosenPieceIndex = bagOfSevenPieces.at(0);
        bagOfSevenPieces.erase(bagOfSevenPieces.begin());

        colorPiece = chosenPieceIndex;
        for (int i = 0; i < 4; i++) {
            currentPiece[i].x = pieces[chosenPieceIndex][i] % 2 + 4;
            currentPiece[i].y = pieces[chosenPieceIndex][i] / 2 + 3;
            currentPiece[i].rotation = 0;

            if (colorPiece == J_TETROMINO) {
                currentPiece[i].x--;
            }
        }

        for (int i = 0; i < 4; i++) {
            Piece center = currentPiece[1];
            if (colorPiece == I_TETROMINO) {
                currentPiece[i].rotate(center, 1);
            }
            if (colorPiece == T_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (colorPiece == S_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (colorPiece == Z_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (colorPiece == L_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (colorPiece == J_TETROMINO) {
                currentPiece[i].rotate(center, 1);
            }
        }

        // Флаги управления
        int isRotateClockWise = 0;
        int isRotateCounterClockwise = 0;
        int isHardDrop = 0;
        int isMoveRight = 0;
        int isMoveLeft = 0;
        int isHoldPiece = 0;
        int isSidehit = 0;
        int isTSpin = 0;
        int isRestart = 0;

        int keyRotateClockwise = 0;
        int keyRotateCounterClockwise = 0;
        int keyHardDrop = 0;
        int keyMoveRight = 0;
        int keyMoveLeft = 0;
        int keyRightHold = 0;
        int keyLeftHold = 0;
        bool isOnFocus = true;
        int lastKeyPressed = 0;
        int keyRestart = 0;

        // Основной цикл обработки событий в игре Тетрис.
        // Цикл будет работать, пока окно открыто. Он обновляет таймер игры, проверяет столкновения,
        // управляет логикой блокировки тетромино и обрабатывает события управления.
        while (gameWindow.isOpen()) {
            float elapsedTime = trackingFrameTime.getElapsedTime().asSeconds();
            
            for (int i = 0; i < 4; i++) {
                previousPiecePosition[i] = currentPiece[i];
                currentPiece[i].y++;
            }

            if (!isCollided()) {
                lockDelayValue -= elapsedTime;

                if (lockDelayValue <= 0) {
                    for (int i = 0; i < 4; i++) {
                        currentPiece[i] = previousPiecePosition[i];
                    }

                    goto makeNewPiece;
                }

                if (!isTouchGround) {
                    isTouchGround = true;
                }
            } else {
                isTouchGround = false;
            }

            for (int i = 0; i < 4; i++) {
                currentPiece[i] = previousPiecePosition[i];
            }

            gameTimer += elapsedTime;
            trackingFrameTime.restart();

            sf::Event e;
          
            while (gameWindow.pollEvent(e)) {
                if (e.type == sf::Event::GainedFocus) {
                    isOnFocus = true;
                }
                if (e.type == sf::Event::LostFocus) {
                    isOnFocus = false;
                }

                if (isOnFocus && e.type == sf::Event::KeyReleased) {
                    if (e.key.code == sf::Keyboard::X || e.key.code == sf::Keyboard::Up) {
                        isRotateClockWise = 0;
                    }
                    if (e.key.code == sf::Keyboard::Z) {
                        isRotateCounterClockwise = 0;
                    }
                    if (e.key.code == sf::Keyboard::Space) {
                        isHardDrop = 0;
                    }
                    if (e.key.code == sf::Keyboard::C) {
                        isHoldPiece = 0;
                    }
                    if (e.key.code == sf::Keyboard::Left) {
                        isMoveLeft = 0;
                    }
                    if (e.key.code == sf::Keyboard::Right) {
                        isMoveRight = 0;
                    }
                    if (e.key.code == sf::Keyboard::R) {
                        isRestart = 0;
                    }
                }

            
                if (e.type == sf::Event::Closed || e.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::MainMenu;
                    gameWindow.close(); 
                    return;
                }

            }

            if (isOnFocus) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && isHoldPiece == 0) {
                    isHoldPiece = -1;
                    holded = 1;
                }

                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && isRotateClockWise == 0) {
                    isRotateClockWise = -1;
                    rotatePiece = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && isRotateCounterClockwise == 0) {
                    isRotateCounterClockwise = -1;
                    rotatePiece = -1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isHardDrop == 0) {
                    isHardDrop = -1;
                    hardDrop = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && isMoveLeft == 0) {
                    isMoveLeft = -1;
                    moveLeft = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && isMoveRight == 0) {
                    isMoveRight = -1;
                    moveRight = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && isRestart == 0) {
                    isRestart = -1;
                    goto restart;
                }

                

                /// \brief Проверяет, проиграл ли игрок, и обрабатывает ускоренное падение.
                ///
                /// Эта часть кода проверяет, находится ли игрок в "мертвом" состоянии 
                /// (когда больше нет доступных ходов или фигуры заполнили игровое поле),
                /// и в случае смерти перезапускает игру. Также обрабатывается ввод с клавиши
                /// "стрелка вниз", которая активирует ускоренное падение фигуры.
                /// 
                /// \details Если игрок считается мертвым (функция isDead() возвращает true), 
                /// то игра перезапускается (идет переход к метке restart). Если нажата клавиша 
                /// "стрелка вниз", то устанавливается ускоренное время падения фигуры.
                /// 
                /// \note Использование метки restart позволяет быстро перезапустить игру без
                /// повторного создания объектов.
                /// 
                /// Управление клавишами:
                /// - Стрелка вниз: Ускоренное падение фигуры.
                if (isDead()) {
                    currentState = GameState::GameOver;
                    isGameOver = true;
                    gameWindow.close();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    gameDelay = fastDropDelay;
                }


                /// \brief Обработка движений фигуры влево, вправо и мягкого падения.
                ///
                /// Этот блок кода обрабатывает перемещения текущей фигуры по горизонтали
                /// (влево и вправо) и вычисляет мягкое падение в воздухе. Он проверяет, нажаты
                /// ли клавиши движения и выполняет сдвиг фигуры на одну клетку в соответствующем
                /// направлении. Также осуществляется проверка на столкновения и обработка
                /// блокировки фигуры, если движение невозможно.
                ///
                /// \details
                /// - Если нажаты клавиши влево или вправо, фигура сдвигается на одну клетку
                ///   по оси X. Если после этого она сталкивается с другими блоками или стенками,
                ///   фигура возвращается на исходное место.
                /// - В случае столкновения, если не завершился отсчет времени до блокировки (lock),
                ///   уменьшается счетчик lockCountValue и сбрасывается таймер задержки блокировки.
                /// - Когда время мягкого падения (airSoftDrop) заканчивается, фигура падает
                ///   по направлению к низу, если нет препятствий.
                ///
                /// \note Переменная `airSoftDropValue` контролирует скорость мягкого падения,
                ///       в то время как переменные `lockCountValue` и `lockDelayValue` 
                ///       управляют процессом блокировки фигуры на месте.
                keyMoveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                keyMoveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                if (!keyMoveLeft && !keyMoveRight) {
                    airSoftDropValue = airSoftDrop;
                }

                if (moveRight) {
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].x += 1;
                    }

                    if (!isCollided()) for (int i = 0; i < 4; i++) {
                        currentPiece[i] = previousPiecePosition[i];
                    } else {
                        if (lockCountValue > 0) {
                            lockCountValue--;
                            lockDelayValue = lockDelay;
                        }
                    }

                    airSoftDropValue = airSoftDrop;
                }

                if (moveLeft) {
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].x -= 1;
                    }

                    if (!isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    } else {
                        if (lockCountValue > 0) {
                            lockCountValue--;
                            lockDelayValue = lockDelay;
                        }
                    }

                    airSoftDropValue = airSoftDrop;
                }

                if (airSoftDropValue <= 0) {
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].x += sign(keyMoveRight - keyMoveLeft);
                    }

                    if (!isCollided()) {
                        if (isSidehit == 0) {
                            isSidehit = 1;
                        }

                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    } else {
                        isSidehit = 0;
                        if (lockCountValue > 0) {
                            lockCountValue--;
                            lockDelayValue = lockDelay;
                        }
                    }
                }

                airSoftDropValue -= trackingFrameTime.getElapsedTime().asSeconds() * 75;

                /// \brief Обработка механики удержания фигуры.
                ///
                /// Этот блок кода реализует механику удержания текущей фигуры и замены её на
                /// фигуру из запаса. Если фигура ещё не была удержана, она помещается в запас,
                /// а текущая фигура заменяется на новую. Если фигура уже была удержана, она
                /// заменяется на фигуру, находящуюся в запасе.
                ///
                /// \details
                /// - Если фигура впервые удерживается (переменная holdPiece равна -1), она
                ///   заменяется на новую фигуру, а текущая помещается в запас.
                /// - Если фигура уже была удержана, происходит их обмен.
                /// - После удержания фигуры проверяются столкновения, и, если это необходимо,
                ///   фигура поднимается, чтобы избежать пересечений с другими блоками или стенами.
                /// - После выполнения удержания, обновляются параметры блокировки (lock),
                ///   перезапускается игровой таймер и происходит перерисовка новой фигуры.
                ///
                /// \note Этот блок также управляет вращением фигуры после удержания в зависимости
                ///       от типа фигуры (например, для T, Z или I тетромино применяются разные
                ///       вращения).
                if (holded) {
                    if (holdPiece == -1) {

                        holdPiece = colorPiece;
                        colorPiece = -1;

                        while (isCollided()) {
                            for (int i = 0; i < 4; i++) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].y--;
                        }

                        lockDelayValue = -1;
                        gameTimer = 0;
                        goto makeNewPiece;
                    } else {
                        int temp;
                        temp = colorPiece;
                        colorPiece = holdPiece;
                        holdPiece = temp;

                        gameTimer = 0;
                        lockCountValue = lockCount;
                        lockDelayValue = lockDelay;

                        chosenPieceIndex = colorPiece;
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].x = pieces[chosenPieceIndex][i] % 2 + 4;
                            currentPiece[i].y = pieces[chosenPieceIndex][i] / 2 + 2;
                            currentPiece[i].rotation = 0;

                            if (colorPiece == J_TETROMINO) {
                                currentPiece[i].x--;
                            }
                            if (colorPiece == I_TETROMINO) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            Piece center = currentPiece[1];
                            if (colorPiece == I_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            if (colorPiece == T_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == S_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == Z_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == L_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == J_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            currentPiece[i].rotation = 1;
                        }
                    }
                }

                /// \brief Обработка жёсткого падения фигуры (Hard Drop).
                ///
                /// Этот блок кода реализует механику жёсткого падения фигуры, при котором она
                /// мгновенно опускается на самое нижнее возможное положение на игровом поле.
                ///
                /// \details
                /// - Когда игрок активирует жёсткое падение (переменная hardDrop равна true),
                ///   фигура опускается до тех пор, пока не столкнётся с другими блоками или нижней границей поля.
                /// - Каждый шаг падения сопровождается генерацией частиц для визуализации.
                /// - После того как фигура достигла нижней позиции, она немного поднимается
                ///   вверх, чтобы откорректировать столкновение.
                /// - После этого сбрасывается задержка блокировки (lockDelayValue),
                ///   и происходит переход к созданию новой фигуры.
                ///
                /// \note Жёсткое падение значительно ускоряет процесс игры, так как мгновенно
                ///       завершает движение фигуры до её фиксации на поле.
                if (hardDrop && start <= 0) {
                    while (isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].y++;
                        }
                        createParticle(&particles);
                    }

                    for (int i = 0; i < 4; i++) {
                        currentPiece[i].y--;
                    }
                    lockDelayValue = -1;
                    goto makeNewPiece;
                }

                /// \brief Обработка вращения фигуры.
                ///
                /// Этот блок кода реализует механику вращения текущей фигуры, учитывая
                /// её тип и возможные столкновения с другими блоками или границами игрового поля.
                ///
                /// \details
                /// - Если активировано вращение (rotatePiece не равно 0), сохраняется текущее положение
                ///   фигуры перед вращением.
                /// - Для фигуры типа I_TETROMINO вращение происходит с учетом её текущей ориентации.
                /// - Для других типов фигур вращение происходит с использованием метода rotate,
                ///   который принимает центр вращения и направление вращения.
                /// - После вращения проверяется возможность перемещения фигуры.
                /// - Если фигура не может быть перемещена, выполняется проверка, является ли фигура
                ///   T_TETROMINO и подсчитываются возможные углы для определения T-Spin.
                ///
                /// \note Для фигуры I_TETROMINO используется специальная логика вращения,
                ///       учитывающая ее уникальную форму.
                if (rotatePiece != 0) {
                    int beforeRotation, afterRotation;
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        beforeRotation = currentPiece[i].rotation;

                       if (colorPiece == I_TETROMINO) {
                           switch (currentPiece[i].rotation)
                           {
                           case 1:
                               if (rotatePiece == 1) {
                                   if (i == 0) {
                                       currentPiece[i].x -= 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 2) {
                                       currentPiece[i].x += 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x += 2, currentPiece[i].y += 2;
                                   }
                               }
                               else {
                                   if (i == 0) {
                                       currentPiece[i].x -= 2, currentPiece[i].y += 2;
                                   }
                                   if (i == 1) {
                                       currentPiece[i].x -= 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x += 1, currentPiece[i].y -= 1;
                                   }
                               }
                               break;

                           case 2:
                               if (rotatePiece == 1) {
                                   if (i == 0) {
                                       currentPiece[i].x -= 2, currentPiece[i].y += 2;
                                   }
                                   if (i == 1) {
                                       currentPiece[i].x -= 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x += 1, currentPiece[i].y -= 1;
                                   }
                               }
                               else {
                                   if (i == 0) {
                                       currentPiece[i].x += 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 2) {
                                       currentPiece[i].x -= 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x -= 2, currentPiece[i].y -= 2;
                                   }
                               }
                               break;

						   case 3:
                               if (rotatePiece == 1) {
                                   if (i == 0) {
                                       currentPiece[i].x += 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 2) {
                                       currentPiece[i].x -= 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x -= 2, currentPiece[i].y -= 2;
                                   }
                               }
                               else {
                                   if (i == 0) {
                                       currentPiece[i].x += 2, currentPiece[i].y -= 2;
                                   }
                                   if (i == 1) {
                                       currentPiece[i].x += 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x -= 1, currentPiece[i].y += 1;
                                   }
                               }
							   break;
							   
						   case 4:
                               if (rotatePiece == 1) {
                                   if (i == 0) {
                                       currentPiece[i].x += 2, currentPiece[i].y -= 2;
                                   }
                                   if (i == 1) {
                                       currentPiece[i].x += 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x -= 1, currentPiece[i].y += 1;
                                   }
                               }
                               else {
                                   if (i == 0) {
                                       currentPiece[i].x -= 1, currentPiece[i].y -= 1;
                                   }
                                   if (i == 2) {
                                       currentPiece[i].x += 1, currentPiece[i].y += 1;
                                   }
                                   if (i == 3) {
                                       currentPiece[i].x += 2, currentPiece[i].y += 2;
                                   }
                               }
                               break;
						   
                           default:
                               break;
                           }

                            currentPiece[i].rotation += rotatePiece;
                        } else if (colorPiece != O_TETROMINO) {
                            currentPiece[i].rotate(currentPiece[1], rotatePiece);
                        }

                        if (currentPiece[i].rotation > 4) {
                            currentPiece[i].rotation = 1;
                        }
                        if (currentPiece[i].rotation < 1) {
                            currentPiece[i].rotation = 4;
                        }

                        afterRotation = currentPiece[i].rotation;
                    }

                    /// \brief Обработка вращения фигуры.
                    /// 
                    /// Этот блок кода реализует механику вращения текущей фигуры, учитывая
                    /// её тип и возможные столкновения с другими блоками или границами игрового поля.
                    /// 
                    /// \details
                    /// - Если активировано вращение (rotatePiece не равно 0), сохраняется текущее положение
                    ///   фигуры перед вращением.
                    /// - Для фигуры типа I_TETROMINO вращение происходит с учетом её текущей ориентации.
                    /// - Для других типов фигур вращение осуществляется с использованием метода rotate,
                    ///   который принимает центр вращения и направление вращения.
                    /// - После вращения проверяется возможность перемещения фигуры.
                    /// - Если фигура не может быть перемещена, выполняется проверка, является ли фигура
                    ///   T_TETROMINO, и подсчитываются возможные углы для определения T-Spin.
                    /// 
                    /// \note Для фигуры I_TETROMINO используется специальная логика вращения,
                    ///       учитывающая её уникальную форму.

                    Piece rotationPieces[4];
                    if (colorPiece != I_TETROMINO) {
                        if ((beforeRotation == 1 && afterRotation == 2) || (beforeRotation == 3 && afterRotation == 2)) {
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    rotationPieces[i].x = currentPiece[i].x;
                                    rotationPieces[i].y = currentPiece[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += 0;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += -1;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 0;
                                    currentPiece[i].y += 2;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += 2;
                                }
                            }
                        }

                        if ((beforeRotation == 2 && afterRotation == 1) || (beforeRotation == 2 && afterRotation == 3)) {
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    rotationPieces[i].x = currentPiece[i].x;
                                    rotationPieces[i].y = currentPiece[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += 0;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += 1;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 0;
                                    currentPiece[i].y += -2;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {

                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += -2;
                                }
                            }
                        }

                        if ((beforeRotation == 3 && afterRotation == 4) || (beforeRotation == 1 && afterRotation == 4)) {
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    rotationPieces[i].x = currentPiece[i].x;
                                    rotationPieces[i].y = currentPiece[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += 0;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += -1;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 0;
                                    currentPiece[i].y += 2;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 1;
                                    currentPiece[i].y += 2;
                                }
                            }
                        }

                        if ((beforeRotation == 4 && afterRotation == 3) || (beforeRotation == 4 && afterRotation == 1)) {
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    rotationPieces[i].x = currentPiece[i].x;
                                    rotationPieces[i].y = currentPiece[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += 0;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += 1;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += 0;
                                    currentPiece[i].y += -2;
                                }
                            }
                            if (!isCollided()) {
                                for (int i = 0; i < 4; i++) {
                                    currentPiece[i].x = rotationPieces[i].x;
                                    currentPiece[i].y = rotationPieces[i].y;
                                    currentPiece[i].x += -1;
                                    currentPiece[i].y += -2;
                                }
                            }
                        }
                    }

                    if (!isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    } else {
                        if (lockCountValue > 0) {
                            lockCountValue--;
                            lockDelayValue = lockDelay;
                        }

                        if (colorPiece == T_TETROMINO) {
                            int corner_count = 0;
                            if (board[currentPiece[1].y + 1][currentPiece[1].x + 1] != 0 || currentPiece[1].x + 1 >= WIDTH || currentPiece[1].y + 1 >= HEIGHT) {
                                corner_count++;
                            }
                            if (board[currentPiece[1].y + 1][currentPiece[1].x - 1] != 0 || currentPiece[1].x - 1 < 0 || currentPiece[1].y + 1 >= HEIGHT) {
                                corner_count++;
                            }
                            if (board[currentPiece[1].y - 1][currentPiece[1].x + 1] != 0 || currentPiece[1].x + 1 >= WIDTH || currentPiece[1].y - 1 < 0) {
                                corner_count++;
                            }
                            if (board[currentPiece[1].y - 1][currentPiece[1].x - 1] != 0 || currentPiece[1].x - 1 < 0 || currentPiece[1].y + 1 < 0) {
                                corner_count++;
                            }

                            if (corner_count >= 3) {
                                isTSpin = 1;
                            }
                        }
                    }
                }
                
                /// \brief Обновление игры.
                /// 
                /// Этот блок кода выполняет обновление состояния игры, включая перемещение текущей фигуры
                /// вниз по игровому полю и проверку на столкновения. Если фигура достигла дна или 
                /// других блоков, происходит её фиксация на игровом поле и генерация новой фигуры.
                /// 
                /// \details
                /// - Если таймер игры превышает заданную задержку, текущая фигура перемещается вниз.
                /// - Проверяется возможность перемещения фигуры; если столкновений нет, 
                ///   фиксируются её координаты и происходит создание эффектов частиц.
                /// - В случае, если фигура фиксируется на поле, обновляется состояние игрового поля,
                ///   добавляются фиксированные блоки, и определяется новая фигура из мешка семи.
                /// - Проверяется возможность очистки линий, подсчитывается количество очищенных линий
                ///   и обновляются соответствующие статистики.
                /// - Если фигурa не может быть перемещена из-за столкновения, её положение возвращается
                ///   к предыдущему состоянию.
                /// 
                /// \note При очистке линий учитываются специальные комбинации, такие как T-Spin.
                if (gameTimer > gameDelay) {
                makeNewPiece:
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].y++;
                    }

                    if (!isCollided() && lockDelayValue < 0) {
                        if (!holded) {
                            createParticle(&particles);
                            createParticle(&particles);
                            createParticle(&particles);

                            std::vector<PieceLock> pieceLockList;
                            for (int i = 0; i < 4; i++) {
                                PieceLock piece_lock_one;
                                piece_lock_one.x = currentPiece[i].x;
                                piece_lock_one.y = currentPiece[i].y;

                                pieceLockList.push_back(piece_lock_one);
                            }
                            piecesLock.push_back(pieceLockList);

                            boardWobble = 7;
                            totalPieceCount++;
                        }

                        for (int i = 0; i < 4; i++) {
                            board[previousPiecePosition[i].y][previousPiecePosition[i].x] = colorPiece + 1;
                        }

                        int selectedPiece = bagOfSevenPieces.at(0);
                        bagOfSevenPieces.erase(bagOfSevenPieces.begin());
                        
                        if (bagOfSevenPieces.size() == 0) {
                            bagOfSevenPieces = nextBagOfSevenPieces;
                            nextBagOfSevenPieces = generateNewBag();
                        }

                        colorPiece = selectedPiece;
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].x = pieces[selectedPiece][i] % 2 + 4;
                            currentPiece[i].y = pieces[selectedPiece][i] / 2 + 2;
                            currentPiece[i].rotation = 0;

                            if (colorPiece == J_TETROMINO) {
                                currentPiece[i].x--;
                            }
                            if (colorPiece == I_TETROMINO) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            Piece center = currentPiece[1];
                            if (colorPiece == I_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            if (colorPiece == T_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == S_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == Z_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == L_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (colorPiece == J_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            currentPiece[i].rotation = 1;
                        }

                        lockDelayValue = lockDelay;
                        lockCountValue = lockCount;
                        isTouchGround = false;

                        /// \brief Обновление состояния игры.
                        ///
                        /// Этот блок кода отвечает за обновление состояния игрового процесса, включая
                        /// передвижение фигуры вниз, проверку столкновений, создание новой фигуры, 
                        /// блокировку фигуры на месте, очистку линий и сброс счетчиков.
                        int lineClearCount = 0;
                        int checkLine = HEIGHT - 1;
                        for (int i = HEIGHT - 1; i > 0; i--) {
                            int count = 0;
                            for (int j = 0; j < WIDTH; j++) {
                                if (board[i][j]) count++;
                                board[checkLine][j] = board[i][j];
                            }

                            if (count < WIDTH) {
                                checkLine--;
                            } else {
                                lineClearCount++;
                            }
                        }

                        if (lineClearCount != 0) {
                            lineClearCombo++;

                            score += calculateScore(lineClearCount, lineClearCombo);

                            if (lineClearCount == 4 || isTSpin) {
                                btbCombo++;
                            } else {
                                if (btbCombo != 0) {
                                    btbCombo = 0;
                                }
                            }

                            totalLineCount += lineClearCount;
                        } else if (lineClearCombo != 0) {
                            lineClearCombo = 0;
                        }

                        int perfectClear = 1;
                        for (int i = 0; i < HEIGHT; i++) {
                            for (int j = 0; j < WIDTH; j++) {
                                if (board[i][j] != 0) {
                                    perfectClear = 0;
                                }
                            }
                        }
                        isTSpin = 0;
                    } else if (!isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    } else {
                        lockCountValue = lockCount;

                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                            createParticle(&particles);
                            createParticle(&particles);
                        }
                    }

                    gameTimer = 0;
                }
                boardWobble += 100 * ((-boardWobble) / 15) * elapsedTime;

                /// \brief Обновление и отображение "Призрака" фигуры.
                /// 
                /// Этот блок кода отвечает за обновление "призрака" (Ghost) — прозрачного отображения фигуры,
                /// показывающего её будущее положение при быстром падении (Hard Drop).
                /// 
                /// \details 
                /// - Призрак копирует координаты текущей фигуры.
                /// - Призрак движется вниз, пока не столкнётся с другими блоками или границами.
                /// - После достижения нижнего положения, фигура-призрак поднимается на одну клетку, чтобы не пересекаться с коллизиями.
                for (int i = 0; i < 4; i++) {
                    ghost[i].x = currentPiece[i].x;
                    ghost[i].y = currentPiece[i].y;
                }

                for (int i = 0; i < 4; i++) {
                    while (isCollidedGhost()) {
                        for (int i = 0; i < 4; i++) {
                            ghost[i].y++;
                        }
                    }

                    for (int i = 0; i < 4; i++) {
                        ghost[i].y--;
                    }
                }

                moveX = 0;
                rotatePiece = 0;
                gameDelay = 0.5;
                hardDrop = 0;
                holded = 0;
                moveLeft = 0;
                moveRight = 0;

                gameWindow.clear(sf::Color::Black);
                //в Метод Draw 
                /// \brief Отрисовка заднего фона.
                /// 
                /// Этот блок кода отрисовывает заднюю панель игрового поля (Backboard).
                sf::RectangleShape backboardShape;
                backboardShape.setSize(sf::Vector2f(320, 650));
                backboardShape.setFillColor(sf::Color::White);
                backboardShape.setPosition(140, 20 + boardWobble);
                gameWindow.draw(backboardShape);

                /// \brief Отрисовка сетки игрового поля.
                /// 
                /// Этот блок кода отвечает за отображение сетки, на которой происходят игровые действия.
                spriteBoard.setPosition(150, -30 + boardWobble);
                gameWindow.draw(spriteBoard);

                /// \brief Отрисовка полосы DAS (Delayed Auto Shift).
                /// 
                /// Полоса DAS отображает прогресс задержки перед перемещением фигуры
                /// при удерживании клавиши влево или вправо.
                float dasProgress = std::max((double)0, (double)(lockDelayValue / lockDelay));

                sf::RectangleShape dasBarShape;
                dasBarShape.setSize(sf::Vector2f(dasProgress * 320, 8));
                dasBarShape.setFillColor(sf::Color::White);
                dasBarShape.setPosition(140, 700 + boardWobble);
                gameWindow.draw(dasBarShape);

                /// \brief Отрисовка удерживаемой фигуры (Hold).
                /// 
                /// Этот блок отвечает за отображение удерживаемой фигуры в отдельной области экрана.
                /// 
                /// \details
                /// - Если фигура удерживается (holdPiece != -1), копируются её координаты и отрисовываются.
                /// - Положение удерживаемой фигуры скорректировано для некоторых фигур (например, I_TETROMINO, J_TETROMINO).
                sf::CircleShape lockCountCircle;
                lockCountCircle.setRadius(6);
                lockCountCircle.setFillColor(sf::Color::White);
                for (int i = 0; i < lockCountValue; i++) {
                    lockCountCircle.setPosition(141 + (i * 23.5), 680 + boardWobble);
                    gameWindow.draw(lockCountCircle);
                }

                /// \brief Отрисовка удерживаемой фигуры (Hold).
                /// 
                /// Этот блок отвечает за отображение удерживаемой фигуры в отдельной области экрана.
                /// 
                /// \details
                /// - Если фигура удерживается (holdPiece != -1), копируются её координаты и отрисовываются.
                /// - Положение удерживаемой фигуры скорректировано для некоторых фигур (например, I_TETROMINO, J_TETROMINO).
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

                /// \brief Отрисовка следующих фигур (Next Pieces).
                ///
                /// Отображает до пяти следующих фигур, которые будут выпадать в игре.
                spritePiece.setColor(sf::Color(255, 255, 255, 255));
                for (int i = 0; i < bagOfSevenPieces.size() + nextBagOfSevenPieces.size(); i++) {
                    if (i < 5) {
                        Piece nextPiece[4];
                        int nextPieceChoose;
                        if (i < bagOfSevenPieces.size()) {
                            nextPieceChoose = bagOfSevenPieces.at(i);
                        } else {
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

                /// \brief Отрисовка размещенных на поле фигур.
                ///
                /// Проходим по игровому полю и отрисовываем уже размещенные на нем фигуры.
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

                /// \brief Отрисовка анимации блокировки фигур.
                ///
                /// Фигуры, которые были заблокированы, анимируются и постепенно исчезают.
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

                /// \brief Отрисовка призрачной фигуры (Ghost Piece).
                spriteGhost.setTextureRect(sf::IntRect(colorPiece * TEXTURE_SIZE, 0, TEXTURE_SIZE, TEXTURE_SIZE));
                for (int i = 0; i < 4; i++) {
                    spriteGhost.setPosition(currentPiece[i].x * TEXTURE_SIZE + 150, ghost[i].y * TEXTURE_SIZE - 90 + boardWobble);
                    gameWindow.draw(spriteGhost);
                }

                /// \brief Отрисовка текущей фигуры (Current Piece).
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

                /// \brief Отрисовка частиц.
                ///
                /// Частицы, создаваемые при различных эффектах, таких как исчезновение фигур.
                for (int i = 0; i < particles.size(); i++) {
                    particles.at(i).update(elapsedTime);
                    particles.at(i).draw(&gameWindow);
                    if (particles.at(i).alpha <= 0) {
                        particles.erase(particles.begin() + i);
                    }
                }

                /// \brief Отрисовка фона.
                ///
                /// 
                /// Панель отображает элементы интерфейса, такие как очки и индикаторы.
                backboardShape.setSize(sf::Vector2f(320, 20));
                backboardShape.setFillColor(sf::Color::Black);
                backboardShape.setPosition(140, 0 + boardWobble);
                gameWindow.draw(backboardShape);

                backboardShape.setSize(sf::Vector2f(320, 10));
                backboardShape.setFillColor(sf::Color::White);
                backboardShape.setPosition(140, 10 + boardWobble);
                gameWindow.draw(backboardShape);

                /// \brief Отрисовка текстовых элементов интерфейса игры.
                ///
                /// Этот блок кода отвечает за отображение текста, включая "NEXT", "HOLD", количество линий и фигур.
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

                gameWindow.display();

            }
            start--;
            trackingGameTime.restart();
        }
}