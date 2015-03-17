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

bool GameState::isSolution() {
	//TODO: returns whether all required (+prereq) courses have semesters assigned to it
	//check if its valid though
	return true;
}

bool GameState::isValid() {
	//checks the constraints: pre-reqs, cannot retake a course, budget, Cmin, and Cmax

	//pre-reqs: check to see if the pre-req of a course is taken for every course
	//TODO: consider only checking the latest assignment i.e. this->courseList[courseList.size()-1]
	for(unsigned int i = 0; i < courseList.size(); i++)
	{
		Course* curr_course = this->courseList[i]; //the course to check if pre-reqs are met as a pointer
		int curr_semester = curr_course->semesterID; //the semester the current course is to be assigned
		unsigned int num_curr_prereqs = curr_course->prereqList.size(); //the number of prereqs
		if(num_curr_prereqs == 0) //if there are no prereqs, all prereqs are satisfied
		{
			break;
		}
		else
		{
			//iterator through prereqlist for curr_course
			for(vector<Course*>::iterator it = curr_course->prereqList.begin(); it!=curr_course->prereqList.end(); ++it)
			{
				//get the semesterID for each prereq
				//check to see if it's assigned (not less than 0) and if its semester ID is less than curr_semester
				int temp_semester = (*it)->semesterID; //note that it is a pointer to a Course pointer, have to dereference it and use '->'
				if(temp_semester >= 0 && temp_semester < curr_semester)
				{
					//condition met, prereq assigned and taken
					continue;
				}
				else
				{
					//condition not met, prereq either not assigned or not taken prior to curr_semester
					continue;
				}
				continue; 
			}


		}


	}

	return true;
}
