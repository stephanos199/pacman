#pragma once
#include <SFML\Graphics.hpp>

namespace BFS
{
	class Node {
	private:
		sf::Vector2i position;
		Node* parent;
	public:
		Node(sf::Vector2i pos, Node* parent);
		sf::Vector2i getPosition();
		Node* getParent();
	};
};

