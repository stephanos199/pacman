#include "GBFS.h"

GBFS::Node::Node(sf::Vector2i pos, Node * parent, double distanceToGoal) 
	: position(pos), parent(parent), distanceToGoal(distanceToGoal)
{
}

sf::Vector2i GBFS::Node::getPosition()
{
	return position;
}

GBFS::Node* GBFS::Node::getParent() {
	return parent;
}

float GBFS::Node::getDistanceToGoal() const {
	return distanceToGoal;
}

double GBFS::VectorDistance(sf::Vector2i source, sf::Vector2i dest) {
	// Euclidean
	double dx = source.x - dest.x;
	double dy = source.y - dest.y;
	return sqrt(dx+dx + dy*dy);
}