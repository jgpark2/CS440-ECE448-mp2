#ifndef GAMETREE_H
#define GAMETREE_H

#include <vector>
#include "board.h"

/*
 * gameTree class that represents the Game Tree used to backtrack.
 * contains gameStates in its nodes
 */
class GameTree {
	public:
		GameTree();
		~GameTree();
		GameTree(Board* root);
		
		bool isEmpty();
		void printTree();
		void clearTree();
		
		Board* root;
		
};

#endif
