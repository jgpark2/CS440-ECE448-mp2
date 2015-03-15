#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "course.h"
#include <vector>

/*
 * gameState class that represents the Game State. These are nodes to the game tree.
 */
class GameState {
	public:
		GameState();
		GameState(GameState const &gs);
		~GameState();
		void setCoursesFromVector(std::vector<Course*> courseList);
		
		std::vector<Course*> courseList;
		
};

#endif
