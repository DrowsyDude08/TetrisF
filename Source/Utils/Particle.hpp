#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/Graphics.hpp>

class Particle {
public:
    //Инициализирует новую частицу с заданным положением, скоростью и направлением.
    Particle(int x, int y, int speed, int direction);

    double x;
    double y;
    int direction;
    double speed;
    double alpha = 255;
    sf::RectangleShape shape;

    void update(double timePerFrame);
    void draw(sf::RenderWindow* window);
};

#endif // PARTICLE_HPP
