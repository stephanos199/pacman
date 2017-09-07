#pragma once
#include <SFML\Graphics.hpp>

namespace GBFS
{
	class Node {
	private:
		float distanceToGoal;
		sf::Vector2i position;
		Node* parent;
	public:
		Node(sf::Vector2i pos, Node* parent, double distanceToGoal);
		sf::Vector2i getPosition();
		Node* getParent();
		float getDistanceToGoal() const;
	};
	struct NodeComparator {
		bool operator()(const Node * a, const Node * b) {
			return a->getDistanceToGoal() < b->getDistanceToGoal();
		}
	};
	double VectorDistance(sf::Vector2i source, sf::Vector2i dest);
};

