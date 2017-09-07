#include "AStar.h"
#include <cmath>

AStar::Node::Node(sf::Vector2i coords, Node * parent, double gCost, double hCost)
	: coords(coords), parent(parent), gCost(gCost), hCost(hCost)
{
	fCost = gCost + hCost;
}
AStar::Node::Node()
{
	Node(sf::Vector2i(), 0, 0, 0);
}
AStar::Node::~Node() {}
double AStar::Node::estimatedCostToGoal(Node & goal)
{
	// Manhattan
	return abs(coords.x - goal.coords.x) + abs(coords.y - goal.coords.y);
}
bool AStar::Node::isGoal(Node & goal) {
	return coords == goal.coords;
}
AStar::Node* AStar::Node::getParent() {
	return parent;
}
double AStar::Node::getFCost()
{
	return fCost;
}
double AStar::Node::getGCost()
{
	return gCost;
}
void AStar::Node::setParent(Node* parent) {
	this->parent = parent;
}
double AStar::VectorDistance(sf::Vector2i source, sf::Vector2i dest) {
	// Manhattan
	double dx = source.x - dest.x;
	double dy = source.y - dest.y;
	return abs(dx) + abs(dy);
}
bool AStar::vecInList(std::vector<AStar::Node*> list, sf::Vector2i vec) {
	return std::find_if(list.begin(), list.end(), [&](AStar::Node* nd) -> bool {
		return nd->coords == vec;
	}) != list.end();
}
