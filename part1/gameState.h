#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "course.h"
#include "semester.h"
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
		GameState(std::vector<Course*> courseList, int cmin, int cmax, int budget, char mode);
		GameState(GameState const &gs);
		~GameState();
		
		void setCoursesFromVector(std::vector<Course*> courseList);
		void updateAssignment();
		
		GameState* assign(int assignCourseID, int assignSemester);
		
		bool isRoot();
		
		bool isValid();
		bool checkCmin();
		bool isSolution();
		
		bool prereqSatisfied();
		bool validPrereqs(Course* course);
		
		int semesterCredit(int semesterID);
		void printState();
		std::vector<int> leastConstrainingValues(int courseID);
		int mostConstrainedCourse();
		void amplifyRank(std::vector<int> prereq, int depth);
		void amplifyRankMini(std::vector<int> prereq, int depth);
		
		std::vector<GameState*> children; int childrenCount;
		std::vector<Course*> courseList;
		GameState * parent; //pointer to const GS.
		std::map<int, std::vector<Course*> > assignment;
		
		std::vector<Semester*> semesters;
		
		int cmin;
		int cmax;
		int totalBudget;
		int maxSemesterID; //Domain of values
		char mode;
		//int totalCredit; //total credit of all classes thus far
		
};

#endif
