#include "gameState.h"

using namespace std;
using namespace GameState;

//default constructor
GameState() {
	//Empty Game Tree
}

//copy constructor
GameState(GameState const &gs){
	setCoursesFromVector(gs.courseList);
}

//destructor
~GameState() {
	if (courseList!=NULL) {
		for (int i=0; i<courseList.size(); ++i) {
			delete courseList[i];
		}
	}
}


GameState(vector<Course*> courseList) {
	setCoursesFromVector(courseList);
}

void setCoursesFromVector(vector<Course*> courseList){
	//DEEP copy all Course objects into my own list
	for(int i=0; i<courseList.size(); ++i) {
		this->courseList.push_back(new Course(courseList[i]));
	}
}

