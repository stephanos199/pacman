#include "Character.h"

auto height = 20, width = 20;

Character::Character(sf::Vector2i pos): position(pos)
{
}

Character::~Character()
{
}

void Character::setSpriteFromFile(std::string filename)
{
	tx.loadFromFile(filename);
	sprite.setTexture(tx);
	sprite.setPosition(sf::Vector2f(position.x * width + width / 2, position.y * height + height / 2));
	sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
}

void Character::setPosition(sf::Vector2i&& pos)
{
	std::lock_guard <std::mutex> lock(posMutex);
	position = pos;
}

sf::Vector2i Character::getPosition()
{
	std::lock_guard <std::mutex> lock(posMutex);
	return position;
}

void Character::move(sf::Vector2i add)
{
	position += add;
	sprite.setPosition(
		sf::Vector2f(
			position.x * width + sprite.getLocalBounds().width / 2 + 2, 
			position.y * height + sprite.getLocalBounds().height / 2 + 2));
}

sf::Sprite & Character::getSprite()
{
	return sprite;
}
