#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "course.h"
#include <vector>
#include <cstddef>

/*
 * gameState class that represents the Game State. These are nodes to the game tree.
 */
class GameState {
	public:
		GameState();
		GameState(std::vector<Course*> courseList);
		GameState(GameState const &gs);
		~GameState();
		
		void setCoursesFromVector(std::vector<Course*> courseList);
		void addChildGameState(GameState* child);
		bool isRoot();
		
		std::vector<GameState*> children;
		std::vector<Course*> courseList;
		GameState* parent;
		
		
};

#endif
