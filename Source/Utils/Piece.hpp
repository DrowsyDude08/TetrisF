#ifndef PIECE_HPP
#define PIECE_HPP

class Piece {
public:
    int x;
    int y;
    int rotation = 1;

    void rotate(Piece center, int direction);
};

class PieceLock {
public:
    int x;
    int y;
    double animation = 0;
};

#endif // PIECE_HPP
