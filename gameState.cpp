#include "gameState.h"

using namespace std;

//default constructor
GameState::GameState() {
	//Empty Game Tree
}

//copy constructor
GameState::GameState(GameState const &gs){
	setCoursesFromVector(gs.courseList);
}

//destructor
GameState::~GameState() {
	for (unsigned int i=0; i<courseList.size(); ++i) {
		delete courseList[i];
	}
}

GameState::GameState(vector<Course*> courseList) {
	setCoursesFromVector(courseList);
}

bool GameState::isRoot(){
	if (parent==NULL)
		return true;
	return false;
}

void GameState::addChildGameState(GameState* child) {
	children.push_back(child);
	child->parent = this;
}

void GameState::setCoursesFromVector(vector<Course*> courseList){
	//DEEP copy all Course objects into my own list
	for(unsigned int i=0; i<courseList.size(); ++i) {
		this->courseList.push_back(new Course( *(courseList[i]) ));
	}
}
