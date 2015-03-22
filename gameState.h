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
		//default constructor
		//A new blank GameState must be defined with a list of Courses. Or it can be copied from an existing GameState.
		//Thus, there cannot be a default constructor
		GameState(std::vector<Course*> courseList, int cmin, int cmax, int budget);
		GameState(GameState const &gs);
		~GameState();
		
		void setCoursesFromVector(std::vector<Course*> courseList);
		GameState* assign(int assignCourseID, int assignSemester);
		bool isRoot();
		bool isValid();
		bool checkCmin();
		bool isSolution();
		bool prereqSatisfied(Course* course);
		int semesterCredit(int semesterID);
		void updateAssignment();
		void printState();
		
		std::vector<GameState*> children;
		std::vector<Course*> courseList;
		GameState const * parent; //pointer to const GS. You can't modify a parent from a child...
		std::map<int, std::vector<Course*> > assignment;
		
		int cmin;
		int cmax;
		int totalBudget;
		//int totalCredit; //total credit of all classes thus far
		
};

#endif
