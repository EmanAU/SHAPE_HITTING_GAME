#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player {
private:
	sf::CircleShape shape;
	float speed;

public:
	Player(float startX = 0, float startY = 0, float speed = 0.5) {
		shape.setRadius(35);
		shape.setFillColor(sf::Color::Red);
		shape.setOutlineColor(sf::Color::Black);
		shape.setOutlineThickness(5.0f);
		shape.setPosition(startX, startY);
		this->speed = speed;
	}

	void moveLeft() { shape.move(-speed, 0); }
	void moveRight() { shape.move(speed, 0); }
	void moveUp() { shape.move(0, -speed); }
	void moveDown() { shape.move(0, speed); }

	void stayInWindow(float windowWidth, float windowHeight) {
		sf::Vector2f pos = shape.getPosition();
		float radius = shape.getRadius();

		if (pos.x < 0) shape.setPosition(0, pos.y);
		if (pos.x + 2 * radius > windowWidth) shape.setPosition(windowWidth - 2 * radius, pos.y);
		if (pos.y < 0) shape.setPosition(pos.x, 0);
		if (pos.y + 2 * radius > windowHeight) shape.setPosition(pos.x, windowHeight - 2 * radius);
	}

	sf::CircleShape getShape() const { return shape; }
	float getRadius() const { return shape.getRadius(); }

	void draw(sf::RenderWindow& window) {
		window.draw(shape);
	}

	sf::FloatRect getBounds() const {
		return shape.getGlobalBounds();
	}

};

#endif
