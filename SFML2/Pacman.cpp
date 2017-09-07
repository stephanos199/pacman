#include "Pacman.h"



Pacman::Pacman(): Character(sf::Vector2i(0,0))
{
	dir = 'L';
	rotation = 180;
	// Center the origin
	//sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}


Pacman::~Pacman()
{
}

char Pacman::getDirection()
{
	return dir;
}

void Pacman::setDirection(char direction) {
	dir = direction;
	switch (dir) {
	case 'L':
		rotation = 180;
		break;
	case 'R':
		rotation = 0;
		break;
	case 'U':
		rotation = 270;
		break;
	case 'D':
		rotation = 90;
		break;
	}
}

void Pacman::move(sf::Vector2i add) {
	Character::move(add);
	sprite.setRotation(rotation);
}