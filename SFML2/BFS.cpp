#include "BFS.h"

BFS::Node::Node(sf::Vector2i pos, Node * parent) : position(pos), parent(parent)
{
}

sf::Vector2i BFS::Node::getPosition()
{
	return position;
}

BFS::Node* BFS::Node::getParent() {
	return parent;
}