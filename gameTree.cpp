#include "gameTree.h"

using namespace std;

//default constructor
GameTree::GameTree() {
	//Empty Game Tree
}

GameTree::GameTree(GameState* root) {
	this->root = root;
}

GameTree::~GameTree() { }

bool GameTree::isEmpty() {
	if (root==NULL)
		return true;
	return false;
}

void GameTree::clearTree() {
	//TODO: free mem -> loop through all nodes and its children and delete them
	root = NULL;
}

void GameTree::printTree() {
	//TODO: whee
}
