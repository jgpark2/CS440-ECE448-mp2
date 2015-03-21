#include "gameState.h"

using namespace std;

//default constructor
GameState::GameState() {
	//Empty Game State
	cout << "A new blank GameState must be defined with a list of Courses. Or it can be copied from an existing GameState." << endl;
	//TODO: prevent.
}

//Copy constructor that actually automatically is set to the child of the copy-ee.... to entice iterative gametree bulding.
//If this is an issue, please rewrite this function and other functions in gameState.cpp
//only deep copies courselist. It does not inherit children. Assignment list is simply recalculated.
//***Parent is set default to what this is being copied from. SemesterID is incremented.
//credit,cmin,cmax,and budget is simply copied
GameState::GameState(GameState const &gs){
	setCoursesFromVector(gs.courseList);
	parent = &gs;

	curSemester = gs.curSemester + 1;
	totalCredit = gs.totalCredit;
	cmin = gs.cmin;
	cmax = gs.cmax;
	curBudget = gs.curBudget;

	//Recalculate assignment list (cannot assign parent's list to mine because then my pointers point to parents' courses)
	updateAssignment();
}

void GameState::updateAssignment() {
	//TODO:Clear the map
	//Do we have to reinitialize the vectors?

	//Loop through courselist and re-populate
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if (courseList[i]->semesterID!=-1)
			assignment[courseList[i]->semesterID].push_back(courseList[i]);
	}
}

//destructor
GameState::~GameState() {
	for (unsigned int i=0; i<courseList.size(); ++i) {
		delete courseList[i];
	}

	//TODO:Do we have to delete assignment vectors itself at each semesterID index?
}

/*
 * Due to GameState copy constructor, this should ideally only be called once for the root (empty gamestate),
 * and have all other ndoes build off of this
 */
GameState::GameState(vector<Course*> courseList) {
	setCoursesFromVector(courseList);
	updateAssignment(); //Unnecessary if caller uses this only for an empty gamestate, but just in case...
}

bool GameState::isRoot(){
	if (parent==NULL)
		return true;
	return false;
}

void GameState::addChildGameState(GameState* child) {
	//TODO: Check for duplicate adds and this children vector
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
	if(isValid) {
		for(unsigned int i=0; i<courseList.size(); ++i) {
			if(courseList[i].interesting && courseList[i].semesterID=-1)
				return false;
		}
		return true;
	}

	return false;
}

bool GameState::isValid() {
	//checks the constraints: pre-reqs, cannot retake a course, budget, Cmin, and Cmax

	/* This is the implementation for non-iterative GameState building.
	This would be when:
	0th GameState starts completely empty
	root GameState of the tree is valid
	all subsequent GameState nodes must be built from its parent, and only if the parent is valid
	//Pre-Req's - check if all prereqs have been met BEFOREHAND
	for(unsigned int i = 0; i < courseList.size(); i++) {
		//Loop through all assigned courses (including this state's semester) and check if all prereqs are met
		if (!prereqSatisfied(courseList[i])) {
			return false;
		}
	}*/

	vector<Course*> semesterCourseList = assignment[semesterID];
	for(vector<Course*>::iterator it = semesterCourseList.begin(); it!=semesterCourseList.end(); ++it) {
		if (!prereqSatisfied(it))
			return false;
	}

	//retake: never assign 2 values to a variable. This must be checked on the assignment step, so the condition is implicitly satisfied

	//budget
	if(curBudget<0)
		return false;

	//Make sure this semester's credit hours are in range
	//TODO: If GameStates are built non-iteratively, you need to instead loop through all semesterID's thus far rather than just checking curSemester
	int curSemesterCredit = semesterCredit(curSemester);
	if(curSemesterCredit<cmin || curSemesterCredit>cmax)
		return false;

	return true;
}

bool GameState::prereqSatisfied(Course* course) {
	for(vector<Course*>::iterator it = course->prereqList.begin(); it!=course->prereqList.end(); ++it) {
		if (it->semesterID==-1 || it->semesterID >= course->semesterID)
			return false;
	}
	return true;
}

int GameState::semesterCredit(int semesterID) {
	vector<Course*> semesterCourseList = assignment[semesterID];
	//TODO: if key has no value mapped to it? If it returns an empty vector, it's fine but... otherwise break out pls
	int credit = 0;
	for(vector<Course*>::iterator it = semesterCourseList.begin(); it!=semesterCourseList.end(); ++it) {
		credit += it->credit;
	}
	return credit;
}

