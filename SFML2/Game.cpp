#include "Game.h"
#include "AStar.h"
#include "BFS.h"
#include "GBFS.h"
#include <queue>
#include <algorithm>
#include <iostream>

Game::Game() : score(0), state('p')
{
	std::ifstream map("pacmap.txt");
	std::string line;
	while (std::getline(map, line)) {
		std::stringstream sstream(line);
		std::vector<int> mapLine;
		int aux;
		while(sstream >> aux) {
			mapLine.push_back(aux);
		}
		mapLayer.push_back(mapLine);
	}

	std::vector<std::vector<int>> characterLayer;
	std::ifstream chars("pacchar.txt");
	while (std::getline(chars, line)) {
		std::stringstream sstream(line);
		std::vector<int> charsLine;
		int aux;
		while (sstream >> aux) {
			charsLine.push_back(aux);
		}
		characterLayer.push_back(charsLine);
	}

	// Look for characters
	for (unsigned int i = 0; i < characterLayer.size(); i++) {
		for (unsigned int j = 0; j < characterLayer[i].size(); j++) {
			switch (characterLayer[i][j]) {
				case 1: // Pac-man
					pacman.setPosition(sf::Vector2i(j, i));
					pacman.setSpriteFromFile("resource/pacman.png");
					break;
				case 2: // blinky
					blinky.setPosition(sf::Vector2i(j, i));
					blinky.setSpriteFromFile("resource/blinky.png");
					break;
				case 3: // clyde
					clyde.setPosition(sf::Vector2i(j, i));
					clyde.setSpriteFromFile("resource/clyde.png");
					break;
				case 4: // inky
					inky.setPosition(sf::Vector2i(j, i));
					inky.setSpriteFromFile("resource/inky.png");
					break;
				case 5: // pinky
					pinky.setPosition(sf::Vector2i(j, i));
					pinky.setSpriteFromFile("resource/pinky.png");
					break;
			}
		}
	}
}
Game::~Game()
{
}
std::vector<std::vector<int>> Game::getMapLayer()
{
	std::lock_guard<std::mutex> lock(mapMutex);
	return mapLayer;
}
std::vector<Ghost*> Game::getGhosts()
{
	return {&blinky, &clyde, &inky, &pinky};
}
Pacman* Game::getPacman()
{
	return &pacman;
}
int Game::getScore()
{
	return score;
}
int Game::getMapCell(int x, int y)
{
	if (x < 0 ||
		x > mapLayer[0].size() ||
		y < 0 ||
		y > mapLayer.size()) {
		return 0;
	}
	std::lock_guard<std::mutex> lock(mapMutex);
	return mapLayer[y][x];
}
void Game::setMapCell(int x, int y, int val)
{
	std::lock_guard<std::mutex> lock(mapMutex);
	mapLayer[y][x] = val;
}
void Game::processInput()
{
	auto pacmanPosition = pacman.getPosition();
	char pacmanDir = pacman.getDirection();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (getMapCell(pacmanPosition.x - 1, pacmanPosition.y) != 0) {
			pacmanDir = 'L';
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (getMapCell(pacmanPosition.x + 1, pacmanPosition.y) != 0) {
			pacmanDir = 'R';
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		if (getMapCell(pacmanPosition.x, pacmanPosition.y - 1) != 0) {
			pacmanDir = 'U';
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		if (getMapCell(pacmanPosition.x, pacmanPosition.y + 1) != 0) {
			pacmanDir = 'D';
		}
	}
	pacman.setDirection(pacmanDir);
}
int Game::movePacman()
{
	int delay = 0;
	auto position = pacman.getPosition();
	sf::Vector2i moveVec(0, 0);
	switch (pacman.getDirection()) {
		case 'L':
			moveVec.x--;
			break;
		case 'R':
			moveVec.x++;
			break;
		case 'U':
			moveVec.y--;
			break;
		case 'D':
			moveVec.y++;
			break;
	}

	auto moveTo = position + moveVec;
	switch (getMapCell(moveTo.x, moveTo.y)) {
		case 1:
			setMapCell(moveTo.x, moveTo.y, 2);
			pacman.move(moveVec);
			score += 10;
			delay = 45;
			break;
		case 2:
			pacman.move(moveVec);
			delay = 0;
			break;
	}
	// change numbers for enum
	// check for ghosts
	return delay;
}
void Game::moveBlinky() { // Stephanos
	using namespace AStar;

	auto cmp = [](Node* a, Node* b) {
		return a->getFCost() < b->getFCost();
	};
	auto mapWidth = mapLayer[0].size();
	auto mapHeight = mapLayer.size();
	
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	auto start = Node(blinky.getPosition(), nullptr, 0, VectorDistance(blinky.getPosition(), pacman.getPosition()));
	
	Node* currentNode = &start;
	openList.push_back(currentNode);

	while (!openList.empty()) {
		currentNode = openList[0];
		if (currentNode->coords == pacman.getPosition()) {
			// Return
			std::vector<Node*> path;
			while (currentNode->getParent() != nullptr) {
				path.push_back(currentNode);
				currentNode = currentNode->getParent();
			}
			if (path.empty()) {
				return;
			}
			sf::Vector2i blinkyPos = blinky.getPosition();
			sf::Vector2i nextPos = path.back()->coords;
			sf::Vector2i moveTo = nextPos - blinkyPos;
			blinky.move(moveTo);
			return;
		}

		for (int i = 1; i < 9; i+=2) {
			int x = currentNode->coords.x;
			int y = currentNode->coords.y;
			int xi = (i % 3) - 1;
			int yi = (i / 3) - 1;
			auto cell = getMapCell(x + xi, y + yi);
			if (cell == 0) {
				continue;
			}
			sf::Vector2i cellVec(x + xi, y + yi);
			double gCost = currentNode->getGCost() + 1;
			double hCost = VectorDistance(cellVec, pacman.getPosition());
			Node* node = new Node(cellVec, currentNode, gCost, hCost);
			if (vecInList(closedList, cellVec) && gCost >= currentNode->getGCost()) {
				continue;
			}
			if (!vecInList(openList, cellVec) || gCost < currentNode->getGCost()) {
				openList.push_back(node);
			}
		}

		std::sort(openList.begin(), openList.end(), cmp);
		for (auto i = openList.begin(); i != openList.end(); i++) {
			if ((*i)->coords == currentNode->coords) {
				i = openList.erase(i);
				if (i == openList.end()) break;
			}
		}
		closedList.push_back(currentNode);
	}
}
void Game::moveClyde() { // Cesar
	using namespace BFS;
	int mapHeight = mapLayer.size();
	int mapWidth = mapLayer[0].size();
	std::queue<Node*> queue;
	Node* current = new Node(clyde.getPosition(), nullptr);
	std::vector<bool> visited(mapHeight*mapWidth);
	visited[current->getPosition().y * mapWidth + current->getPosition().x] = true;
	queue.push(current);

	while (!queue.empty()) {
		current = queue.front();
		queue.pop();

		if (current->getPosition() == pacman.getPosition()) {
			std::vector<Node*> path;
			Node* node = current;
			while (node->getParent()) {
				path.push_back(node);
				node = node->getParent();
			}
			if (path.empty()) return;
			sf::Vector2i clydePos = clyde.getPosition();
			sf::Vector2i nextPos = path.back()->getPosition();
			sf::Vector2i moveTo = nextPos - clydePos;
			clyde.move(moveTo);
			return;
		}

		int x = current->getPosition().x;
		int y = current->getPosition().y;
		for (int i = 1; i < 9; i+=2) {
			int xi = (i % 3) - 1;
			int yi = (i / 3) - 1;
			if (getMapCell(x + xi, y + yi) == 0) {
				continue;
			}
			sf::Vector2i position(x + xi, y + yi);
			//check that its not visited
			int visitedIndex = position.y * mapWidth + position.x;
			if (visited[visitedIndex]) {
				continue;
			}
			Node* adjacent = new Node(position, current);
			visited[visitedIndex] = true;
			queue.push(adjacent);
		}
	}
}
void Game::moveInky() { // Mariano
	auto currentNode = inky.getPosition();
	auto availableNodes = std::vector<sf::Vector2i>();

	for (int i = 1; i < 9; i += 2) {
		int x = currentNode.x;
		int y = currentNode.y;
		int xi = (i % 3) - 1;
		int yi = (i / 3) - 1;
		auto cell = getMapCell(x + xi, y + yi);
		if (cell == 0) {
			continue;
		}
		else {
			availableNodes.push_back(currentNode + sf::Vector2i(xi, yi));
		}
	}

	auto smallestDist = 9999;
	auto moveTo = availableNodes[0];
	for (auto node : availableNodes) {
		if (AStar::VectorDistance(node, pacman.getPosition()) < smallestDist) {
			smallestDist = AStar::VectorDistance(node, pacman.getPosition());
			moveTo = node;
		}
	}
	inky.move(moveTo - inky.getPosition());
}
void Game::movePinky() { // Steve
	using namespace GBFS;
	int mapHeight = mapLayer.size();
	int mapWidth = mapLayer[0].size();
	std::priority_queue < Node*, std::vector<Node*>, NodeComparator> queue;
	Node* current = new Node(pinky.getPosition(), nullptr, 0);
	std::vector<bool> visited(mapHeight*mapWidth);
	visited[current->getPosition().y * mapWidth + current->getPosition().x] = true;
	queue.push(current);

	while (!queue.empty()) {
		current = queue.top();
		queue.pop();

		if (current->getPosition() == pacman.getPosition()) {
			std::vector<Node*> path;
			Node* node = current;
			while (node->getParent()) {
				path.push_back(node);
				node = node->getParent();
			}
			if (path.empty()) return;
			sf::Vector2i pinkyPos = pinky.getPosition();
			sf::Vector2i nextPos = path.back()->getPosition();
			sf::Vector2i moveTo = nextPos - pinkyPos;
			pinky.move(moveTo);
			return;
		}

		int x = current->getPosition().x;
		int y = current->getPosition().y;
		for (int i = 1; i < 9; i += 2) {
			int xi = (i % 3) - 1;
			int yi = (i / 3) - 1;
			if (getMapCell(x + xi, y + yi) == 0) {
				continue;
			}
			sf::Vector2i position(x + xi, y + yi);
			//check that its not visited
			int visitedIndex = position.y * mapWidth + position.x;
			if (visited[visitedIndex]) {
				continue;
			}
			Node* adjacent = new Node(position, current, VectorDistance(current->getPosition(), pacman.getPosition()));
			visited[visitedIndex] = true;
			queue.push(adjacent);
		}
	}
}
bool Game::ghostHittingPacman() {
	if (inky.getPosition() == pacman.getPosition()) return true;
	if (pinky.getPosition() == pacman.getPosition()) return true;
	if (blinky.getPosition() == pacman.getPosition()) return true;
	if (clyde.getPosition() == pacman.getPosition()) return true;
	return false;
}
void Game::setState(char x) {
	state = x;
}
char Game::getState() {
	return state;
}
void Game::startPacThread(sf::RenderWindow& window)
{
	std::thread pac_man([&]() {
		while (window.isOpen()) {
			// agregar delay cuando pacman come algo
			int delay = movePacman();
			//std::cout << this->getPacman()->getPosition().x << " - " <<  this->getPacman()->getPosition().y << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200 + delay));
		}
	});
	std::thread pac_dead([&](){
		while (window.isOpen()){
			if (ghostHittingPacman()) {
				this->setState('d');
			}
			if (this->getScore() == 2880) {
				this->setState('w');
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	});

	pac_dead.detach();
	pac_man.detach();
}
void Game::startGhostThreads(sf::RenderWindow& window) {
	// Create threads
	std::thread blinky([&]() {
		while (window.isOpen()) {
			moveBlinky(); // A* 
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	});

	std::thread clyde([&]() {
		while (window.isOpen()) {
			moveClyde(); // BFS
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	});

	std::thread inky([&]() {
		while (window.isOpen()) {
			moveInky();
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	});

	std::thread pinky([&]() {
		while (window.isOpen()) {
			movePinky();
			std::this_thread::sleep_for(std::chrono::milliseconds(250));
		}
	});

	// Detach threads
	blinky.detach();
	clyde.detach();
	inky.detach();
	pinky.detach();
}
