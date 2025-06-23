#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>

class Bullet {
private:
	sf::RectangleShape shape;
	float speedY;

public:
	Bullet() {
		shape.setSize(sf::Vector2f(10, 20));
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(-100.0f, -100.0f);
		speedY = 0.0f;
	}

	void shootFrom(float x, float y, float speed) {
		shape.setPosition(x, y);
		speedY = speed;
	}

	void update() {
		shape.move(0, -speedY);
	}

	bool isOffScreen() {
		return (shape.getPosition().y + shape.getSize().y < 0);
	}

	void draw(sf::RenderWindow& window) {
		window.draw(shape);
	}

	sf::Vector2f getPosition() const {
		return shape.getPosition();
	}

	sf::FloatRect getBounds() const
	{
		return shape.getGlobalBounds();
	}

	void deactivate()
	{
		shape.setPosition(-100.0f, -100.0f);
		speedY = 0;
	}

};

#endif
