#ifndef ENEMY_H
#define ENEMY_H
#include <SFML/Graphics.hpp>

class Enemy {
protected:
	sf::Shape* shape;
	float speed;
	bool isDying = false;
	float transparency = 255;

public:
	Enemy(float spd)
	{
		speed = spd;
	}


	virtual void update() = 0;
	virtual void draw(sf::RenderWindow& window) = 0;

	virtual sf::Vector2f getPosition() const {
		return shape->getPosition();
	}

	void startDying()
	{
		isDying = true;
	}

	bool removeWithAnimation() {
		if (isDying) {

			transparency -= 10;
			if (transparency < 0) transparency = 0;


			sf::Color col = shape->getFillColor();
			col.a = static_cast<sf::Uint8>(transparency);
			shape->setFillColor(col);


			shape->setScale(shape->getScale().x * 0.95f, shape->getScale().y * 0.95f);

			shape->rotate(5.0f);

			if (transparency <= 0) return true;
		}
		return false;
	}


	bool isDied() const {
		return isDying;
	}

	sf::FloatRect getBounds() const {
		return shape->getGlobalBounds();
	}

	virtual ~Enemy() {
		delete shape;
	}
};



#endif;