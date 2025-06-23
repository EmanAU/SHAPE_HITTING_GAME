#ifndef CIRCLEENEMY_H
#define CIRCLEENEMY_H
#include "Enemy.h"


class CircleEnemy : public Enemy
{
public:

	CircleEnemy(float x, float y, float speed)
		: Enemy(speed) 
	{
		shape = new sf::CircleShape(40.0f);
		shape->setFillColor(sf::Color::Black);
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