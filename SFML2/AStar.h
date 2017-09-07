#pragma once
#include <SFML/Graphics.hpp>
namespace AStar {
	class Node
	{
	private:
		Node* parent;
		double gCost, hCost, fCost;
	public:
		sf::Vector2i coords;
		Node(sf::Vector2i coords, Node* parent, double gCost, double hCost);
		Node();
		~Node();
		double estimatedCostToGoal(Node& goal);
		bool isGoal(Node & goal);
		Node* getParent();
		double getFCost();
		double getGCost();
		void setParent(Node* parent);
	};
	double VectorDistance(sf::Vector2i source, sf::Vector2i dest);
	bool vecInList(std::vector<AStar::Node*> list, sf::Vector2i vec);
}