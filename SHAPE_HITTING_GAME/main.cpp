#include <SFML/Audio.hpp>
#include <iostream>
#include <ctime>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "CircleEnemy.h"
#include "RectEnemy.h"
#include "TriangleEnemy.h"

using namespace std;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int MAX_BULLETS = 1000;
const int MAX_ENEMIES = 5;

void resetGame(Player& player, Bullet bullets[], bool bulletActive[], Enemy* enemies[], bool enemyActive[], sf::Clock& fireClock, sf::Clock& spawnClock, int& score) {
	player = Player(300, 300, 0.5f);

	for (int i = 0; i < MAX_BULLETS; i++) bulletActive[i] = false;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemyActive[i]) {
			delete enemies[i];
			enemyActive[i] = false;
		}
	}

	fireClock.restart();
	spawnClock.restart();
	score = 0;
}

int main() {

	srand(static_cast<unsigned>(time(0)));

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "OOP Shape Shooting Game");
	

	sf::Texture backgroundTexture;
	if (!backgroundTexture.loadFromFile("background.jpg")) {
		cout << "Failed to load background image\n";
	}
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	background.setScale(
		(float)WINDOW_WIDTH / backgroundTexture.getSize().x,
		(float)WINDOW_HEIGHT / backgroundTexture.getSize().y
	);

	sf::Font font;
	if (!font.loadFromFile("ARIAL.TTF")) {
		cout << "Font loading failed.\n";
		return -1;
	}

	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setString("Game Over");
	gameOverText.setCharacterSize(60);
	gameOverText.setFillColor(sf::Color::Red);
	gameOverText.setPosition(500, 250);

	sf::Text optionsText;
	optionsText.setFont(font);
	optionsText.setString("Press R to Restart | Press Q to Quit");
	optionsText.setCharacterSize(30);
	optionsText.setFillColor(sf::Color::White);
	optionsText.setPosition(450, 400);

	sf::Text finalScoreText;
	finalScoreText.setFont(font);
	finalScoreText.setCharacterSize(30);
	finalScoreText.setFillColor(sf::Color::Yellow);
	finalScoreText.setPosition(550, 350);
	finalScoreText.setString("Final Score: 0");


	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(20, 10);
	scoreText.setString("Score: 0");


	Player player(300, 300, 0.78f);
	Bullet bullets[MAX_BULLETS];
	bool bulletActive[MAX_BULLETS] = { false };
	Enemy* enemies[MAX_ENEMIES] = { nullptr };
	bool enemyActive[MAX_ENEMIES] = { false };

	sf::Sound fireSound, hitSound, gameOverSound;
	sf::SoundBuffer fireSoundFile, hitSoundFile, gameOverSoundFile;

	if (!fireSoundFile.loadFromFile("bullet_fire.WAV")) {
		cout << "Failed to load bullet_fire.WAV\n";
	}
	if (!hitSoundFile.loadFromFile("enemyhit.WAV")) {
		cout << "Failed to load enemi_hit_sound.WAV\n";
	}
	if (!gameOverSoundFile.loadFromFile("gameover.WAV")) {
		cout << "Failed to load game_over.WAV\n";
	}

	fireSound.setBuffer(fireSoundFile);
	hitSound.setBuffer(hitSoundFile);
	gameOverSound.setBuffer(gameOverSoundFile);


	sf::Clock fireClock, spawnClock;
	sf::Time fireCooldown = sf::milliseconds(200);
	sf::Time spawnCooldown = sf::milliseconds(500);

	float bulletSpeed = 1.0f;
	float enemySpeed = 0.4f;

	bool gameOver = false;
	int score = 0;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (!gameOver) {
			// Movement Input
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player.moveLeft();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player.moveRight();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.moveUp();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.moveDown();

			player.stayInWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

			// Shooting
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (fireClock.getElapsedTime() > fireCooldown) {
					for (int i = 0; i < MAX_BULLETS; i++) {
						if (!bulletActive[i]) {
							bullets[i].shootFrom(player.getShape().getPosition().x + player.getRadius() - 5,
								player.getShape().getPosition().y,
								bulletSpeed);
							bulletActive[i] = true;
							fireClock.restart();
							fireSound.play();
							break;
						}
					}
				}
			}

			// Move bullets
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bulletActive[i]) {
					bullets[i].update();
					if (bullets[i].isOffScreen()) bulletActive[i] = false;
				}
			}

			// Spawn enemies
			if (spawnClock.getElapsedTime() > spawnCooldown) {
				for (int i = 0; i < MAX_ENEMIES; i++) {
					if (!enemyActive[i]) {
						int type = rand() % 3;
						float x = rand() % (WINDOW_WIDTH - 60);
						if (type == 0)
							enemies[i] = new CircleEnemy(x, -70, enemySpeed);
						else if (type == 1)
							enemies[i] = new RectEnemy(x, -70, enemySpeed);
						else
							enemies[i] = new TriangleEnemy(x, -70, enemySpeed);
						enemyActive[i] = true;
						spawnClock.restart();
						break;
					}
				}
			}

			// Move enemies and check collision with player
			for (int i = 0; i < MAX_ENEMIES; i++) {
				if (enemyActive[i]) {
					enemies[i]->update();
					if (enemies[i]->getPosition().y > WINDOW_HEIGHT) {
						delete enemies[i];
						enemies[i] = nullptr;
						enemyActive[i] = false;
					}

					// Check collision with player
					if (enemies[i] != nullptr) {
						sf::FloatRect enemyBounds = enemies[i]->getBounds();
						sf::FloatRect playerBounds = player.getShape().getGlobalBounds();
						if (enemyBounds.intersects(playerBounds)) {
							gameOver = true;
							finalScoreText.setString("Final Score: " + to_string(score));
							gameOverSound.play();
							break;
						}
					}
				}
			}

			// Bullet-enemy collision
			for (int i = 0; i < MAX_BULLETS; i++) {
				if (bulletActive[i]) {
					for (int j = 0; j < MAX_ENEMIES; j++) {
						if (enemyActive[j] && enemies[j] != nullptr && !enemies[j]->isDied()) {
							if (bullets[i].getBounds().intersects(enemies[j]->getBounds())) {
								bullets[i].deactivate();
								enemies[j]->startDying();
								score += 10;  // Increase score
								break;
							}
						}
					}
				}
			}

			// Enemy fade animation
			for (int i = 0; i < MAX_ENEMIES; i++) {
				if (enemyActive[i] && enemies[i] != nullptr) {
					if (enemies[i]->removeWithAnimation()) {
						delete enemies[i];
						enemies[i] = nullptr;
						enemyActive[i] = false;
					}
				}
			}
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				resetGame(player, bullets, bulletActive, enemies, enemyActive, fireClock, spawnClock, score);
				gameOver = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				window.close();
			}
		}

		// --- Render ---
		window.clear();
		window.draw(background);

		if (!gameOver) {
			scoreText.setString("Score: " + to_string(score));
			player.draw(window);
			window.draw(scoreText);
			for (int i = 0; i < MAX_BULLETS; i++) if (bulletActive[i]) bullets[i].draw(window);
			for (int i = 0; i < MAX_ENEMIES; i++) if (enemyActive[i]) enemies[i]->draw(window);
		}
		else {
			window.draw(gameOverText);
			window.draw(finalScoreText);
			window.draw(optionsText);
		}


		window.display();
	}

	// Clean-up
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i] != nullptr) {
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return 0;
}