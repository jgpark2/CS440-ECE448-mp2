#include "gameState.h"
#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range
#include <vector>         // std::vector
#include <algorithm>    // std::find

using namespace std;

/*
 * Main constructor. Due to GameState copy constructor, this should ideally only be called once
 * for the root (empty gamestate), and have all other ndoes build off of this
 */
GameState::GameState(vector<Course*> courses, int cmin, int cmax, int budget) {
	curSemester = 0;
	totalCredit = 0;
	this->cmin = cmin;
	this->cmax = cmax;
	curBudget = budget; //remaining budget after assigning current semester classes
	parent = NULL;
	
	setCoursesFromVector(courses);
		
	updateAssignment(); //Unnecessary if caller uses this only for an empty gamestate, but just in case...
}

/*
 * Copy constructor that actually automatically is set to the child of the copy-ee....to entice iterative gametree bulding.
 * If this is an issue, please rewrite this function and other functions in gameState.cpp
 * only deep copies courselist. It does not inherit children. Assignment list is simply recalculated.
 * !!!!!!Parent is set default to what this is being copied from. SemesterID is incremented.
 * credit,cmin,cmax,and budget is simply copied
 */
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
	assignment.clear();
	//Do we have to reinitialize the vectors? They should be done automaitcally and the old ones should be destroyed automatically...

	//Loop through courselist and re-populate
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if (courseList[i]->semesterID!=-1)
			assignment.at(courseList[i]->semesterID).push_back(courseList[i]);
	}
}

//destructor
GameState::~GameState() {
	for (unsigned int i=0; i<courseList.size(); ++i) {
		delete courseList[i];
	}

	//TODO:Do we have to delete assignment vectors itself at each semesterID index?
}

bool GameState::isRoot(){
	if (parent==NULL)
		return true;
	return false;
}

void GameState::addChildGameState(GameState* child) {
	if(std::find(children.begin(), children.end(), child) != children.end()) {
		/* Children contains child */
		cout<<"Adding duplicate children. Ignored"<<endl;
		return;
	}
	
	children.push_back(child);
	child->parent = this;
}

void GameState::setCoursesFromVector(vector<Course*> courses){
	courseList.clear();
	
	//DEEP copy all Course objects into my own list
	for(unsigned int i=0; i<courses.size(); ++i) {
		courseList.push_back(new Course( *(courses[i]) ));
	}
}

bool GameState::isSolution() {
	//returns whether all required (+prereq) courses have semesters assigned to it
	if(isValid()) {
		for(unsigned int i=0; i<courseList.size(); ++i) {
			if((courseList[i]->interesting) && (courseList[i]->semesterID)==-1)
				return false;
		}
		return true;
	}

	return false;
}

bool GameState::isValid() {
	//checks the constraints: pre-reqs, cannot retake a course, budget, Cmin, and Cmax

	/* TODO: This is the implementation for non-iterative GameState building.
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

	vector<Course*> semesterCourseList;
	try {
		semesterCourseList = assignment.at(curSemester);
	} catch (const out_of_range& oor) {
		//No courses has been assigned a semesterID in this gamestate!?
		//Technically the cmin constraint would pick this up and return fail but...
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		cout << "You are checking isValid for a gamestate with no semester assignments yet" <<endl;
		return false;
	}
	
	for(vector<Course*>::iterator it = semesterCourseList.begin(); it!=semesterCourseList.end(); ++it) {
		if (!prereqSatisfied(*it))
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
	for(vector<Course*>::iterator it = (course->prereqList).begin(); it!=(course->prereqList).end(); ++it) {
		if ((*it)->semesterID==-1 || (*it)->semesterID >= course->semesterID)
			return false;
	}
	return true;
}

int GameState::semesterCredit(int semesterID) {		
	
	int credit = 0;
	
	vector<Course*> semesterCourseList;
	try {
		semesterCourseList = assignment.at(semesterID);
	} catch (const out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		cout << "Returning 0 credit for yet-to-encounter semesterID" <<endl;
		return credit;
	}
	
	for(vector<Course*>::iterator it = semesterCourseList.begin(); it!=semesterCourseList.end(); ++it) {
		credit += (*it)->credit;
	}
	return credit;
}

void GameState::printState()
{
	//print the map
	if(assignment.empty()) {
		cout << "There are no assignments" << endl;
		return;
	}
	
	//for each semester in the map
	for(map<int, vector<Course*> >::const_iterator it = assignment.begin(); it!=assignment.end(); ++it) {
		cout << "Semester: " << it->first << endl;
		
		//for each course
		for(vector<Course*>::const_iterator it_inner = it->second.begin(); it_inner != it->second.end(); ++it_inner) {
			cout << "\tCourseID: " << (*it_inner)->courseID;
			cout << "\tprice: " << ( ((*it_inner)->semesterID)%2==0 )?((*it_inner)->fallPrice):((*it_inner)->springPrice);
			cout << "\thours: " << (*it_inner)->credit;
			cout << endl;
		}
		cout << endl;
	}
}
