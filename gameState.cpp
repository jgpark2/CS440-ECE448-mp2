#include "gameState.h"
#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range
#include <vector>         // std::vector
#include <algorithm>    // std::find

using namespace std;

/*
 * Main constructor. Due to GameState copy constructor, this should ideally only be called once
 * for the root (empty gamestate), and have all other nodes build off of this via assign(...)
 */
GameState::GameState(vector<Course*> courses, int cmin, int cmax, int budget) {
	this->cmin = cmin;
	this->cmax = cmax;
	totalBudget = budget; //remaining budget after assigning current semester classes
	
	parent = NULL;
	
	setCoursesFromVector(courses);
		
	//updateAssignment(); //Unnecessary if caller uses this only for an empty gamestate
}

/*
 * Copy constructor.
 * deep copies courselist.cmin,cmax,and budget is simply copied.
 * ASSIGNMENT MAP NOT UPDATED (so it's empty)
 * DO NOT CALL THIS. Call assign instead.
 */
GameState::GameState(GameState const &gs){
	setCoursesFromVector(gs.courseList);
	cmin = gs.cmin;
	cmax = gs.cmax;
	totalBudget = gs.totalBudget;
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

/*
 * Creates a new child node (via copy constructor) with new assignments made to it. Returns the new GameState*.
 */
GameState* GameState::assign(int assignCourseID, int assignSemester) {
	
	if(assignCourseID<1 || assignSemester<0) {
		cout << "Invalid assignments"<< endl;
		return NULL;
	}
	
	//Should call copy constructor...
	GameState gs = *this;
	GameState* child = &gs;
	
	children.push_back(child);
	child->parent = this;
	
	//Make assignment
	if (child->courseList[assignCourseID]->semesterID==-1)
		child->courseList[assignCourseID]->semesterID = assignSemester;
	else {
		cout << "Reassignment is illegal. Why are you doing this?" << endl;
		return NULL;
	}
		
	//TODO: assign cred/ decr budget
	
	//Recalculate assignment list (cannot assign parent's list to mine because then my pointers point to parents' courses)
	updateAssignment();
	
	return child;
}


bool GameState::isRoot(){
	if (parent==NULL)
		return true;
	return false;
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
		if (!checkCmin())
			return false;
	
		for(unsigned int i=0; i<courseList.size(); ++i) {
			if((courseList[i]->interesting) && (courseList[i]->semesterID)==-1)
				return false;
		}
		return true;
	}

	return false;
}

bool GameState::checkCmin() {
	for(map<int, vector<Course*> >::const_iterator it = assignment.begin(); it!=assignment.end(); ++it) {
		int credits = 0;
		for(vector<Course*>::const_iterator it_inner = it->second.begin(); it_inner != it->second.end(); ++it_inner) 
			credits+=(*it_inner)->credit;
		
		if(credits<cmin)
			return false;
	}
	return true;
}

bool GameState::isValid() {
	//Checks the entire gamestate (all courses) for the constraints:
	//pre-reqs integrity, budget, Cmin, and Cmax
	
	int budget = 0;
	for(map<int, vector<Course*> >::const_iterator it = assignment.begin(); it!=assignment.end(); ++it) {
		int credits = 0;
		for(vector<Course*>::const_iterator it_inner = it->second.begin(); it_inner != it->second.end(); ++it_inner) {
			//PREREQ
			if (!prereqSatisfied(*it_inner))
				return false;
			
			//CREDIT
			//Make sure current budget doesn't exceed max and credit hours are in range
			credits+=(*it_inner)->credit;
			if(credits>cmax)
				return false;
			
			//BUDGET
			//Find price for given semester type
			if ((*it_inner)->semesterID % 2 ==0) //If FALL
				budget+=(*it_inner)->fallPrice;
			else
				budget+=(*it_inner)->springPrice;
			if (totalBudget-budget<0)
				return false;
		}
	}

	return true;
}

bool GameState::prereqSatisfied(Course* course) {
	//for(vector<int>::iterator it = course->prereqList.begin(); it!=course->prereqList.end(); ++it)
	for(unsigned int i = 0; i < course->prereqList.size(); ++i)
	{
		if(courseList[course->prereqList[i]]->semesterID == -1 || courseList[course->prereqList[i]]->semesterID >= course->semesterID)
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

