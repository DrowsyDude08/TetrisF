#include "GameController.hpp"

GameController::GameController()
    : isGameOver(false),
    gameView(window),
    menuView(window),
    isRotateClockWise(0),
    isRotateCounterClockwise(0),
    isHardDrop(0),
    isMoveRight(0),
    isMoveLeft(0),
    isHoldPiece(0),
    isSidehit(0),
    isTSpin(0),
    score(0),
    isRestart(0),
    keyRotateClockwise(0),
    keyRotateCounterClockwise(0),
    keyHardDrop(0),
    keyMoveRight(0),
    keyMoveLeft(0),
    keyRightHold(0),
    keyLeftHold(0),
    isOnFocus(true),
    lastKeyPressed(0),
    keyRestart(0),
	isTouchGround(false),
    fastDropDelay(0),
    airSoftDrop(8),
    airSoftDropValue(airSoftDrop)

{
    currentState = GameState::MainMenu;
};

void GameController::runGame() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else {
                menuView.handleMainMenuEvent(event);
            }
        }

        window.clear();

        switch (currentState) {
        case GameState::MainMenu:
            menuView.drawMainMenu();
            break;
        case GameState::InGame:
            startTetrisGame();
            break;
        case GameState::GameOver:
            menuView.drawGameOverMenu(leaderboard, score);
            leaderboard.addScore("Игрок 1", score);
            break;
        }

        window.display();
    }
}

std::vector<int> GameController::generateNewBag(){
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

bool GameController::isCollided(){
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
bool GameController::isCollidedGhost(){
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
bool GameController::isDead(){
        for (int i = 0; i < 4; i++) {
            if (board[currentPiece[i].y][currentPiece[i].x]) {
                return true;
            }
        }
        return false;
    }
int GameController::sign(int num){
        if (num < 0) return -1;
        else if (num > 0) return 1;
        else return 0;
    }
int GameController::calculateScore(int lineClearCount, int combo){
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

void GameController::startTetrisGame() {

restart:
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                board[i][j] = 0;
            }
        }

        gameView.start = 1;
        gameTimer = 0;
        gameDelay = 0.5;
        gameView.moveX = 0;
        gameView.colorPiece = 0;
        gameView.hardDrop = 0;
        gameView.rotatePiece = 0;
        gameView.holdPiece = -1;
        gameView.moveLeft = 0;
        gameView.moveRight = 0;
        gameView.holded = 0;
        gameView.bagOfSevenPieces = generateNewBag();
        gameView.nextBagOfSevenPieces = generateNewBag();

        // Выбор первого тетромино
        int chosenPieceIndex = gameView.bagOfSevenPieces.at(0);
        gameView.bagOfSevenPieces.erase(gameView.bagOfSevenPieces.begin());

        gameView.colorPiece = chosenPieceIndex;
        for (int i = 0; i < 4; i++) {
            currentPiece[i].x = pieces[chosenPieceIndex][i] % 2 + 4;
            currentPiece[i].y = pieces[chosenPieceIndex][i] / 2 + 3;
            currentPiece[i].rotation = 0;

            if (gameView.colorPiece == J_TETROMINO) {
                currentPiece[i].x--;
            }
        }

        for (int i = 0; i < 4; i++) {
            Piece center = currentPiece[1];
            if (gameView.colorPiece == I_TETROMINO) {
                currentPiece[i].rotate(center, 1);
            }
            if (gameView.colorPiece == T_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (gameView.colorPiece == S_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (gameView.colorPiece == Z_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (gameView.colorPiece == L_TETROMINO) {
                currentPiece[i].rotate(center, -1);
            }
            if (gameView.colorPiece == J_TETROMINO) {
                currentPiece[i].rotate(center, 1);
            }
        }



        // Основной цикл обработки событий в игре Тетрис.
        // Цикл будет работать, пока окно открыто. Он обновляет таймер игры, проверяет столкновения,
        // управляет логикой блокировки тетромино и обрабатывает события управления.
        while (window.isOpen()) {
            float elapsedTime = gameView.trackingFrameTime.getElapsedTime().asSeconds();

            for (int i = 0; i < 4; i++) {
                previousPiecePosition[i] = currentPiece[i];
                currentPiece[i].y++;
            }

            if (!isCollided()) {
                gameView.lockDelayValue -= elapsedTime;

                if (gameView.lockDelayValue <= 0) {
                    for (int i = 0; i < 4; i++) {
                        currentPiece[i] = previousPiecePosition[i];
                    }

                    goto makeNewPiece;
                }

                if (!isTouchGround) {
                    isTouchGround = true;
                }
            }
            else {
                isTouchGround = false;
            }

            for (int i = 0; i < 4; i++) {
                currentPiece[i] = previousPiecePosition[i];
            }

            gameTimer += elapsedTime;
            gameView.trackingFrameTime.restart();

            sf::Event e;

            while (window.pollEvent(e)) {
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
                    window.close();
                    return;
                }

            }

            if (isOnFocus) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && isHoldPiece == 0) {
                    isHoldPiece = -1;
                    gameView.holded = 1;
                }

                if ((sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && isRotateClockWise == 0) {
                    isRotateClockWise = -1;
                    gameView.rotatePiece = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && isRotateCounterClockwise == 0) {
                    isRotateCounterClockwise = -1;
                    gameView.rotatePiece = -1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isHardDrop == 0) {
                    isHardDrop = -1;
                    gameView.hardDrop = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && isMoveLeft == 0) {
                    isMoveLeft = -1;
                    gameView.moveLeft = 1;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && isMoveRight == 0) {
                    isMoveRight = -1;
                    gameView.moveRight = 1;
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
                    window.close();
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

                if (gameView.moveRight) {
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].x += 1;
                    }

                    if (!isCollided()) for (int i = 0; i < 4; i++) {
                        currentPiece[i] = previousPiecePosition[i];
                    }
                    else {
                        if (gameView.lockCountValue > 0) {
                            gameView.lockCountValue--;
                            gameView.lockDelayValue = gameView.lockDelay;
                        }
                    }

                    airSoftDropValue = airSoftDrop;
                }

                if (gameView.moveLeft) {
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        currentPiece[i].x -= 1;
                    }

                    if (!isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    }
                    else {
                        if (gameView.lockCountValue > 0) {
                            gameView.lockCountValue--;
                            gameView.lockDelayValue = gameView.lockDelay;
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
                    }
                    else {
                        isSidehit = 0;
                        if (gameView.lockCountValue > 0) {
                            gameView.lockCountValue--;
                            gameView.lockDelayValue = gameView.lockDelay;
                        }
                    }
                }

                airSoftDropValue -= gameView.trackingFrameTime.getElapsedTime().asSeconds() * 75;

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
                if (gameView.holded) {
                    if (gameView.holdPiece == -1) {

                        gameView.holdPiece = gameView.colorPiece;
                        gameView.colorPiece = -1;

                        while (isCollided()) {
                            for (int i = 0; i < 4; i++) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].y--;
                        }

                        gameView.lockDelayValue = -1;
                        gameTimer = 0;
                        goto makeNewPiece;
                    }
                    else {
                        int temp;
                        temp = gameView.colorPiece;
                        gameView.colorPiece = gameView.holdPiece;
                        gameView.holdPiece = temp;

                        gameTimer = 0;
                        gameView.lockCountValue = gameView.lockCount;
                        gameView.lockDelayValue = gameView.lockDelay;

                        chosenPieceIndex = gameView.colorPiece;
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].x = pieces[chosenPieceIndex][i] % 2 + 4;
                            currentPiece[i].y = pieces[chosenPieceIndex][i] / 2 + 2;
                            currentPiece[i].rotation = 0;

                            if (gameView.colorPiece == J_TETROMINO) {
                                currentPiece[i].x--;
                            }
                            if (gameView.colorPiece == I_TETROMINO) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            Piece center = currentPiece[1];
                            if (gameView.colorPiece == I_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            if (gameView.colorPiece == T_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == S_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == Z_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == L_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == J_TETROMINO) {
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
                if (gameView.hardDrop && gameView.start <= 0) {
                    while (isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].y++;
                        }
                        gameView.createParticle(&gameView.particles);
                    }

                    for (int i = 0; i < 4; i++) {
                        currentPiece[i].y--;
                    }
                    gameView.lockDelayValue = -1;
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
                if (gameView.rotatePiece != 0) {
                    int beforeRotation, afterRotation;
                    for (int i = 0; i < 4; i++) {
                        previousPiecePosition[i] = currentPiece[i];
                        beforeRotation = currentPiece[i].rotation;

                        if (gameView.colorPiece == I_TETROMINO) {
                            switch (currentPiece[i].rotation)
                            {
                            case 1:
                                if (gameView.rotatePiece == 1) {
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
                                if (gameView.rotatePiece == 1) {
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
                                if (gameView.rotatePiece == 1) {
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
                                if (gameView.rotatePiece == 1) {
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

                            currentPiece[i].rotation += gameView.rotatePiece;
                        }
                        else if (gameView.colorPiece != O_TETROMINO) {
                            currentPiece[i].rotate(currentPiece[1], gameView.rotatePiece);
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
                    if (gameView.colorPiece != I_TETROMINO) {
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
                    }
                    else {
                        if (gameView.lockCountValue > 0) {
                            gameView.lockCountValue--;
                            gameView.lockDelayValue = gameView.lockDelay;
                        }

                        if (gameView.colorPiece == T_TETROMINO) {
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

                    if (!isCollided() && gameView.lockDelayValue < 0) {
                        if (!gameView.holded) {
                            gameView.createParticle(&gameView.particles);


                            std::vector<PieceLock> pieceLockList;
                            for (int i = 0; i < 4; i++) {
                                PieceLock piece_lock_one;
                                piece_lock_one.x = currentPiece[i].x;
                                piece_lock_one.y = currentPiece[i].y;

                                pieceLockList.push_back(piece_lock_one);
                            }
                            gameView.piecesLock.push_back(pieceLockList);

                            gameView.boardWobble = 7;
                            gameView.totalPieceCount++;
                        }

                        for (int i = 0; i < 4; i++) {
                            board[previousPiecePosition[i].y][previousPiecePosition[i].x] = gameView.colorPiece + 1;
                        }

                        int selectedPiece = gameView.bagOfSevenPieces.at(0);
                        gameView.bagOfSevenPieces.erase(gameView.bagOfSevenPieces.begin());

                        if (gameView.bagOfSevenPieces.size() == 0) {
                            gameView.bagOfSevenPieces = gameView.nextBagOfSevenPieces;
                            gameView.nextBagOfSevenPieces = generateNewBag();
                        }

                        gameView.colorPiece = selectedPiece;
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i].x = pieces[selectedPiece][i] % 2 + 4;
                            currentPiece[i].y = pieces[selectedPiece][i] / 2 + 2;
                            currentPiece[i].rotation = 0;

                            if (gameView.colorPiece == J_TETROMINO) {
                                currentPiece[i].x--;
                            }
                            if (gameView.colorPiece == I_TETROMINO) {
                                currentPiece[i].y++;
                            }
                        }

                        for (int i = 0; i < 4; i++) {
                            Piece center = currentPiece[1];
                            if (gameView.colorPiece == I_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            if (gameView.colorPiece == T_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == S_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == Z_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == L_TETROMINO) {
                                currentPiece[i].rotate(center, -1);
                            }
                            if (gameView.colorPiece == J_TETROMINO) {
                                currentPiece[i].rotate(center, 1);
                            }
                            currentPiece[i].rotation = 1;
                        }

                        gameView.lockDelayValue = gameView.lockDelay;
                        gameView.lockCountValue = gameView.lockCount;
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
                            }
                            else {
                                lineClearCount++;
                            }
                        }

                        if (lineClearCount != 0) {
                            gameView.lineClearCombo++;

                            score += calculateScore(lineClearCount, gameView.lineClearCombo);

                            if (lineClearCount == 4 || isTSpin) {
                                gameView.btbCombo++;
                            }
                            else {
                                if (gameView.btbCombo != 0) {
                                    gameView.btbCombo = 0;
                                }
                            }

                            gameView.totalLineCount += lineClearCount;
                        }
                        else if (gameView.lineClearCombo != 0) {
                            gameView.lineClearCombo = 0;
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
                    }
                    else if (!isCollided()) {
                        for (int i = 0; i < 4; i++) {
                            currentPiece[i] = previousPiecePosition[i];
                        }
                    }
                    else {
                        gameView.lockCountValue = gameView.lockCount;

                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                            gameView.createParticle(&gameView.particles);
                        }
                    }

                    gameTimer = 0;
                }
                gameView.boardWobble += 100 * ((-gameView.boardWobble) / 15) * elapsedTime;

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


                window.clear(sf::Color::Black);
                //в Метод Draw 
                /// \brief Отрисовка заднего фона.
                /// 
                /// Этот блок кода отрисовывает заднюю панель игрового поля (Backboard).
                gameView.drawBackground(window);

                /// \brief Отрисовка сетки игрового поля.
                /// 
                /// Этот блок кода отвечает за отображение сетки, на которой происходят игровые действия.
                gameView.drawPlayingField(window);

                /// \brief Отрисовка полосы DAS (Delayed Auto Shift).
                /// 
                /// Полоса DAS отображает прогресс задержки перед перемещением фигуры
                /// при удерживании клавиши влево или вправо.
                gameView.drawDAS(window);

                /// \brief Отрисовка удерживаемой фигуры (Hold).
                /// 
                /// Этот блок отвечает за отображение удерживаемой фигуры в отдельной области экрана.
                /// 
                /// \details
                /// - Если фигура удерживается (holdPiece != -1), копируются её координаты и отрисовываются.
                /// - Положение удерживаемой фигуры скорректировано для некоторых фигур (например, I_TETROMINO, J_TETROMINO).
                gameView.drawHeldTetramino(window);


                /// \brief Отрисовка следующих фигур (Next Pieces).
                ///
                /// Отображает до пяти следующих фигур, которые будут выпадать в игре.
                gameView.drawNextTetraminos(window);

                /// \brief Отрисовка размещенных на поле фигур.
                ///
                /// Проходим по игровому полю и отрисовываем уже размещенные на нем фигуры.
                gameView.drawFieldTetramino(window);

                /// \brief Отрисовка анимации блокировки фигур.
                ///
                /// Фигуры, которые были заблокированы, анимируются и постепенно исчезают.
                gameView.drawLockedTetramino(window);

                /// \brief Отрисовка призрачной фигуры (Ghost Piece).
                gameView.drawGhostTetramino(window);

                /// \brief Отрисовка текущей фигуры (Current Piece).
                gameView.drawCurrentTetramino(window);

                /// \brief Отрисовка частиц.
                ///
                /// Частицы, создаваемые при различных эффектах, таких как исчезновение фигур.
                gameView.drawParticles(window);

                /// \brief Отрисовка фона.
                ///
                /// 
                /// Панель отображает элементы интерфейса, такие как очки и индикаторы.
                gameView.drawBackground(window);

                /// \brief Отрисовка текстовых элементов интерфейса игры.
                ///
                /// Этот блок кода отвечает за отображение текста, включая "NEXT", "HOLD", количество линий и фигур.
                gameView.drawText(window);

                window.display();

            }
            gameView.start--;
            gameView.trackingGameTime.restart();
        }
    }
