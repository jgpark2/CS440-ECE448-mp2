#ifndef GAMETREE_H
#define GAMETREE_H

#include <vector>
#include "gameState.h"

/*
 * gameTree class that represents the Game Tree used to backtrack.
 * contains gameStates in its nodes
 */
class GameTree {
	public:
		GameTree();
		~GameTree();
		GameTree(GameState* root);
		
		bool isEmpty();
		void printTree();
		void clearTree();
		
		GameState* root;
		
};

#endif
