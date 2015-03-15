#include "gameTree.h"

using namespace std;
using namespace GameTree;

//default constructor
GameTree() {
	//Empty Game Tree
}

GameTree(GameState* root) {
	this->root = root;
}

~GameTree() { }

bool isEmpty() {
	(root==NULL)? return true:return false;
}

void clearTree() {
	//TODO: free mem?
	root = NULL;
}

void printTree() {
	//TODO: whee
}
