#ifndef TRIANGLEENEMY_H
#define TRIANGLEENEMY_H
#include "Enemy.h"


class TriangleEnemy : public Enemy
{
public:

	TriangleEnemy(float x, float y, float speed)
		: Enemy(speed)
	{
		sf::ConvexShape* triangle = new sf::ConvexShape(3);
		triangle->setPoint(0, sf::Vector2f(0, 80));
		triangle->setPoint(1, sf::Vector2f(40, 0));
		triangle->setPoint(2, sf::Vector2f(80, 80));
		triangle->setFillColor(sf::Color(71, 51, 7));
		triangle->setPosition(x, y);
		shape = triangle;
	}

	void update() override
	{
		shape->move(0, speed);
	}

	void draw(sf::RenderWindow& window) override
	{
		window.draw(*shape);
	}
};



#endif