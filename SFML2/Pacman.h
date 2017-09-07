#pragma once
#include "Character.h"
class Pacman :
	public Character
{
private:
	char dir;
	double rotation;
public:
	Pacman();
	~Pacman();
	char getDirection();
	void setDirection(char direction);
	void move(sf::Vector2i add);
};

