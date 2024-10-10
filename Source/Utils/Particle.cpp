#include "Particle.hpp"
#include <cmath>
#include <numbers>
Particle::Particle(int x, int y, int speed, int direction) {
    this->x = x;
    this->y = y;
    this->direction = direction;
    this->speed = speed;
}

void Particle::update(double timePerFrame) {
    double angle = this->direction * (std::numbers::pi / 180.0);
    this->x += this->speed * cos(angle) * timePerFrame;
    this->y += this->speed * sin(angle) * timePerFrame;
    this->alpha -= 400 * timePerFrame;
    this->speed += 100 * ((-this->speed) / 15) * timePerFrame;
}

void Particle::draw(sf::RenderWindow* window) {
    this->shape.setSize(sf::Vector2f(2, 2));
    this->shape.setFillColor(sf::Color(255, 255, 255, std::max(0.0, this->alpha)));
    this->shape.setPosition(this->x, this->y);
    window->draw(this->shape);
}
