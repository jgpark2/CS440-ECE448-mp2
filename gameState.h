#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "course.h"
#include <vector>
#include <cstddef>
#include <map>

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
		bool isValid();
		bool isSolution();
		bool prereqSatisfied(Course* course);
		int semesterCredit(int semesterID);
		
		std::vector<GameState*> children;
		std::vector<Course*> courseList;
		GameState* parent;
		std::map<int, std::vector<Course*> > assignment;
		
		int curSemester; //current semester is the last semester the gamestate handled
		int totalCredit; //TODO: DECIDE: toal credit of current semester? or all classes thus far???
		int cmin;
		int cmax;
		int curBudget; //remaining budget after assigning current semester classes
};

#endif
