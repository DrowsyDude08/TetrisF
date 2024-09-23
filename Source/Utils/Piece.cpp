#include "Piece.hpp"

void Piece::rotate(Piece center, int direction) {
    int offsetX = this->y - center.y;
    int offsetY = this->x - center.x;
    this->x = center.x + (offsetX * -direction);
    this->y = center.y + (offsetY * direction);

    this->rotation += direction;

    if (this->rotation > 4) this->rotation = 1;
    if (this->rotation < 1) this->rotation = 4;
}
