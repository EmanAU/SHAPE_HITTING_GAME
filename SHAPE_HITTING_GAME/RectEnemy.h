#ifndef RECTENEMY_H
#define RECTENEMY_H
#include "Enemy.h"

class RectEnemy : public Enemy 
{
public:
    RectEnemy(float x, float y, float speed)
        : Enemy(speed) {
        shape = new sf::RectangleShape(sf::Vector2f(80, 80));
        shape->setFillColor(sf::Color::Green);
        shape->setPosition(x, y);
    }

    void update() override {
        shape->move(0, speed);
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(*shape);
    }
};


#endif