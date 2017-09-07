#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <mutex>
#include "Pacman.h"
#include "Ghost.h"

class Game
{
private:
	int score;
	char state;
	std::vector<std::vector<int>> mapLayer;
	std::mutex mapMutex;
	Pacman pacman;
	Ghost blinky;
	Ghost clyde;
	Ghost inky;
	Ghost pinky;
public:
	Game();
	~Game();
	std::vector<std::vector<int>> getMapLayer();
	std::vector<Ghost*> getGhosts();
	Pacman* getPacman();
	int getScore();
	int getMapCell(int x, int y);
	void setMapCell(int x, int y, int val);
	void processInput();
	int movePacman();
	void moveBlinky();
	void moveClyde();
	void moveInky();
	void movePinky();
	void setState(char x);
	char getState();
	bool ghostHittingPacman();
	void startPacThread(sf::RenderWindow& window);
	void startGhostThreads(sf::RenderWindow & window);

};