#include <SFML/Graphics.hpp>
#include "Game.h"
#include <thread>
#include <iostream>
#include <chrono>

extern const float height = 20;
extern const float width = 20;

int main()
{
	// Window Setup
	sf::Image icono;
	icono.loadFromFile("resource/upc.jpg");
	sf::RenderWindow window(sf::VideoMode(28*width, 31*height + 40), "Pacman");
	window.setPosition(sf::Vector2i(0, 0));
	window.setIcon(icono.getSize().x, icono.getSize().y, icono.getPixelsPtr());
	window.setFramerateLimit(60);

	//Lose/Win images
	sf::Texture txt;
	txt.loadFromFile("resource/youlose.jpeg");
	sf::Sprite youlose(txt);
	youlose.setScale(1.2, 2);
	int lastScore = 0;

	// Score Font
	sf::Font font;
	font.loadFromFile("resource/Pacmania.ttf");

	// Game
	Game game;
	game.startPacThread(window);
	game.startGhostThreads(window);
	
	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		
		game.processInput();

		auto map = game.getMapLayer();
		sf::RectangleShape rect{ sf::Vector2f(width, height) };
		rect.setFillColor(sf::Color::Blue);
		sf::CircleShape point{ (width / 6) };
		point.setFillColor(sf::Color::Yellow);
		sf::CircleShape bigPoint{ (width / 5) };
		bigPoint.setFillColor(sf::Color::Yellow);
		for (unsigned int i = 0; i < map.size(); i++) {
			for (unsigned int j = 0; j < map[i].size(); j++) {
				if (map[i][j] == 0) {
					rect.setPosition(sf::Vector2f(j*width, i*height));
					window.draw(rect);
				}
				else if (map[i][j] == 1) {
					point.setPosition(sf::Vector2f(j*width + width / 2 - point.getRadius(), i*height + height / 2 - point.getRadius()));
					window.draw(point);
				}
				else if (map[i][j] == 2) {
					bigPoint.setPosition(sf::Vector2f(j*width + width / 2 - bigPoint.getRadius(), i*height + height / 2 - bigPoint.getRadius()));
					window.draw(point);
				}
			}
		}

		for (auto ghost : game.getGhosts()) {
			window.draw(ghost->getSprite());
		}

		window.draw(game.getPacman()->getSprite());

		sf::Text score{ sf::String(std::string("Score: ") + std::to_string(lastScore)), font, 30 };
		score.setPosition(5, height * 31);
		score.setFillColor(sf::Color::White);
		window.draw(score);
		
		if (game.getState() == 'd') {
			window.clear(sf::Color::Yellow);
			window.draw(youlose);
			window.draw(score);
		}
		if (game.getState() == 'w') {
			window.clear(sf::Color::Yellow);
		}

		window.display();
		window.clear(sf::Color::Black);
		if (game.getState() == 'p') {
			lastScore = game.getScore();
		}
	}
	return 0;
}