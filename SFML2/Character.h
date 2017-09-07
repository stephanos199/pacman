#pragma once
#include <SFML\Graphics.hpp>
#include <mutex>

class Character
{
private:
	sf::Vector2i position;
	std::mutex posMutex;
	sf::Texture tx;
protected:
	sf::Sprite sprite;
public:
	Character(sf::Vector2i pos);
	~Character();
	void setSpriteFromFile(std::string filename);
	void setPosition(sf::Vector2i&& pos);
	sf::Vector2i getPosition();
	virtual void move(sf::Vector2i add);
	sf::Sprite& getSprite();
};

