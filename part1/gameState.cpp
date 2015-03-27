#include "gameState.h"
#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range
#include <vector>         // std::vector
#include <algorithm>    // std::find
#include <limits>
#include <stdio.h>

using namespace std;

/*
 * Main constructor. Due to GameState copy constructor, this should ideally only be called once
 * for the root (empty gamestate), and have all other nodes build off of this via assign(...)
 */
GameState::GameState(vector<Course*> courses, int cmin, int cmax, int budget, char mode) {
	this->cmin = cmin;
	this->cmax = cmax;
	maxSemesterID = courses.size()-1; //SemesterID starts at 0 (hence, -1)
	totalBudget = budget; //remaining budget after assigning current semester classes

	this->mode = mode;
		
	parent = NULL;
	
	setCoursesFromVector(courses);
	
	//updateAssignment(); //Unnecessary if caller uses this only for an empty gamestate
	
	semesterHead = new Semester(0,NULL,NULL);
	Semester* temp = semesterHead;
	for(int i=1; i<=maxSemesterID; ++i) {
		temp->next = new Semester(i,NULL,NULL);
		temp->next->prev = temp;
		temp = temp->next;
	}
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
	maxSemesterID = gs.maxSemesterID;
	totalBudget = gs.totalBudget;
	mode = gs.mode;
	
	semesterHead = new Semester(0,NULL,NULL);
	Semester* temp = semesterHead;
	for(int i=1; i<=maxSemesterID; ++i) {
		temp->next = new Semester(i,NULL,NULL);
		temp->next->prev = temp;
		temp = temp->next;
	}
}

void GameState::updateAssignment() {
	assignment.clear();
	//Do we have to reinitialize the vectors? They should be done automaitcally and the old ones should be destroyed automatically...

	//Loop through courselist and re-populate
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if (courseList[i]->semesterID!=-1) {
			//map.operator[] declares value automatically on nonexistent keys
			(assignment[courseList[i]->semesterID]).push_back(courseList[i]);
			
			//Semester crp
			Semester* temp = semesterHead;
			for(int j=0; j<courseList[i]->semesterID; ++j)
				temp = temp->next;
			temp->addCourse(courseList[i]);
		}
	}
}

//destructor
GameState::~GameState() {
	for (unsigned int i=0; i<courseList.size(); ++i) {
		delete courseList[i];
	}


	//Delete all chain of pointers to semester...
	Semester* temp_old = semesterHead;
	Semester* temp;
	while (temp!=NULL) {
		temp=temp_old->next;
		delete temp_old;
		temp_old = temp;
	}
	
	//Do we have to delete assignment vectors itself at each semesterID index?
}

/*
 * Creates a new child node (via copy constructor) with new assignments made to it. Returns the new GameState*.
 */
GameState* GameState::assign(int assignCourseID, int assignSemester) {
	
	if(assignCourseID<1 || assignSemester<0) {
		cout << "Invalid assignments"<< endl;
		return NULL;
	}
	
	//Early check to avoid expensive copy constructor if possible
	if (courseList[assignCourseID-1]->semesterID!=-1) {
		cout << "Reassignment is illegal." << endl;
		return NULL;
	}
	
	
	GameState* child = new GameState(*this);
	
	children.push_back(child);
	child->parent = this;
	
	//Make assignment
	if (child->courseList[assignCourseID-1]->semesterID==-1) {
		child->courseList[assignCourseID-1]->semesterID = assignSemester;
	}
	else {
		cout << "This should never happen unless the copy constructor messes up." << endl;
		return NULL;
	}
	
	//Recalculate assignment list (cannot assign parent's list to mine because then my pointers point to parents' courses)
	child->updateAssignment();
	
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
	
	if(!isValid()) //Must be checked first
		return false;
		
	if (!checkCmin())
		return false;
	
	if (!prereqSatisfied())
		return false;
	
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if((courseList[i]->interesting) && (courseList[i]->semesterID)==-1)
			return false;
	}
	
	return true;
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
			//PREREQ IS NOT CHECKED
			//This is because semester assignments are NOT done in incremental order in the algorithm
			//Meaning we can have unassigned prereqs at any point
			//The only true constraint, is then to check if all prereqs have been taken
			//   validly taken (prior to the course at hand) at Solution-Checking stage
			/*
			if (!prereqSatisfied(*it_inner)) {
				cout<<"prereq FAILED"<<endl;//////////////////////////////////
				return false;
			}*/
			//Instead, we check this for short-circuiting:
			if (!validPrereqs(*it_inner))
				return false;
			
			//CREDIT
			//Make sure current budget doesn't exceed max and credit hours are in range
			credits+=(*it_inner)->credit;
			if(credits>cmax) {
				//cout<<"toomanycredit FAILED"<<endl;//////////////////////////////////
				return false;
			}
			
			if (mode=='A')
				continue; //Mode A ignores budget and price
				
			//BUDGET
			//Find price for given semester type
			if ((*it_inner)->semesterID % 2 ==0) //If FALL
				budget+=(*it_inner)->fallPrice;
			else
				budget+=(*it_inner)->springPrice;
			
			if (totalBudget-budget<0) {
				//cout<<"budget FAILED"<<endl;//////////////////////////////////
				return false;
			}
		}
	}

	return true;
}

bool GameState::prereqSatisfied() {
	// Amongst actual function that this is intended for, below also includes a strong prereq *valid* check. This should ideally be in isValid to minimize node expansions, but we couldn't fit it there...
	for(unsigned int c = 0; c < courseList.size(); ++c) {
		if (!(courseList[c]->interesting) && (courseList[c]->semesterID<0)) {
			//Non-needed courses' prereqs need not be checked, unless we actually used this as a filler...
			continue;
		}
			
		for(unsigned int i = 0; i < courseList[c]->prereqList.size(); ++i)
		{
			int prereqID = courseList[c]->prereqList[i];
			if(courseList[prereqID-1]->semesterID == -1) { //Doesn't neccessarily have to check for integrity since validPrereqs already checks it
				return false;
			}
		}
	}
	
	return true;
}

//Only checks if an assigned course & its *assigned* prereqs are valid
bool GameState::validPrereqs(Course* course) {
	if(course->semesterID==0 && course->prereqList.size()>0)
		return false; //shortcircuit: you cant have this on first semester if there's prereqs
		
	for(unsigned int i = 0; i < course->prereqList.size(); ++i)
	{
		if(courseList[course->prereqList[i]-1]->semesterID==-1) //Need to skip yet-to-be-assigned...
			continue;
		
		if(courseList[course->prereqList[i]-1]->semesterID >= course->semesterID) {
			//cout << course->courseID << " taken before " << courseList[course->prereqList[i]-1]->courseID << "!?" << endl;//////////////////////////////////
			return false;
		}
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
	/* PRETTY DEBUG PRINTS */
	//print the map
	if(assignment.empty()) {
		cout << "There are no assignments" << endl;
		return;
	}
	
	int printBudget = totalBudget;
	Semester* curSem = semesterHead;
	
	//for each semester in the map
	for(map<int, vector<Course*> >::const_iterator it = assignment.begin(); it!=assignment.end(); ++it) {
		cout << "Semester: #" << it->first;
		if(it->first % 2 == 0)
			cout<< " FALL\t";
		else cout<<" SPRING\t";
		
		cout<<"("<<(it->second).size()<<" classes) Credits Taken: "<< semesterCredit(it->first)<<"\t";
		cout<<"Budget Change: "<<printBudget<<"->"<< (printBudget-curSem->budget) << "(Used: " << curSem->budget << ")"<<endl;
		printBudget -= curSem->budget;
		
		vector<Course*> assignedList = it->second;
		//for each course
		for(unsigned int i=0; i<assignedList.size(); ++i) {
			cout << "\tCourseID: " << assignedList[i]->courseID <<endl;
			cout << "\tFprice: " << assignedList[i]->fallPrice << endl;
			cout << "\tSprice: " << assignedList[i]->springPrice << endl;
			if ((assignedList[i]->semesterID)%2==0 )
				cout << "\ttaken price: " << (assignedList[i]->fallPrice);
			else
				cout << "\ttaken price: " << (assignedList[i]->springPrice);
			cout << endl;
			cout << "\tsemesterID: " << assignedList[i]->semesterID << endl;
			cout << "\thours: " << assignedList[i]->credit << endl;
			cout << endl;
		}
		curSem = curSem->next;
	}
	
	
	/* FINAL PRINTS AS PRE MP2 SPECS */
	cout << "FINAL:" << endl;
	cout << totalBudget-printBudget << " " << curSem->semesterID << endl;
	Semester* tempSem = semesterHead;
	while(tempSem->courses.size()!=0) {
		cout << tempSem->courses.size() << " ";
		for (vector<Course*>::iterator it=tempSem->courses.begin(); it != tempSem->courses.end(); it++)
			cout << (*it)->courseID << " ";
		cout << endl;
		tempSem = tempSem->next;
	}
	
	tempSem = semesterHead;
	while(tempSem->courses.size()!=0) {
		cout << tempSem->budget << " ";
		tempSem = tempSem->next;
	}
	cout << endl;
}

int GameState::mostConstrainedCourse() {
	//Reset RANK...
	for(unsigned int i=0; i<courseList.size(); ++i) {
		courseList[i]->constrained_rank = 0;
	}
	
	//Calculate RANK
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if(courseList[i]->interesting) {
			courseList[i]->constrained_rank += 10;
			amplifyRank(courseList[i]->prereqList, 1);
		}
	}
	
	//Find max rank...
	int courseID = -1;
	int maxReqRank = -1;
	int leastOfLeast= std::numeric_limits<int>::max();
	
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if ( courseList[i]->semesterID==-1 && courseList[i]->constrained_rank > maxReqRank) {
			courseID = i+1; //COURSEID STARTS AT 1
			maxReqRank = courseList[i]->constrained_rank;
		}
		else if ( courseList[i]->semesterID==-1 && courseList[i]->constrained_rank == maxReqRank) {
			//Tie breakers must be done via who "best-fits" in the cmin~cmax window most for the "lowest unfulfilled semester"
			Semester* temp = semesterHead;
			Semester* leastSem = NULL;
			int least = std::numeric_limits<int>::max();
			for(int j=0; j<= maxSemesterID; ++j) {
				if (temp->credit >= cmin)
					continue;
					
				int weight = temp->credit+courseList[i]->credit-cmin;
				
				if (weight<0)
					weight*=-(cmax-cmin+1);
				else if (weight>cmax-cmin)
					weight=std::numeric_limits<int>::max();
			
				if (weight < least) {
					least = weight;
					leastSem = temp;
				}
				temp = temp->next;
			}
			if (leastSem==NULL)
				continue;//FUCK
			
			if(least<leastOfLeast) {
				leastOfLeast = least;
				courseID=i+1;
			}
		}
	}
	
	
	if (courseID!=-1)////////////////////
		cout<<"MOST CONST: " << courseID << "   ";	////////////////
	
	return courseID;
}

void GameState::amplifyRank(vector<int> prereq, int depth) {
	if(prereq.size()==0)
		return;
		
	depth++;
	
	for(unsigned int i=0; i<prereq.size(); ++i) {
		courseList[prereq[i]-1]->constrained_rank += depth*10;
		amplifyRank(courseList[prereq[i]-1]->prereqList, depth);
	}
	
	return;
}

vector<int> GameState::leastConstrainingValues(int courseID) {
	
	Semester* semPtr = semesterHead;
	for(int j=0; j<=maxSemesterID; ++j) {
		semPtr->visited_flag = false;
		semPtr = semPtr->next;
	}
	
	//Return list of semesterIDs, sorted from least constraining to most constraining
	vector<int> semesterIDs;
	
	while (semesterIDs.size() <= (unsigned int) maxSemesterID) {
		//While semesterIDs isn't fully populated...
		
		int leastConstraint = std::numeric_limits<int>::max();
		Semester* leastSemester = NULL;
		
		//Find the next least Semester
		Semester* temp = semesterHead;
		for(int j=0; j<= maxSemesterID; ++j) {
			if (!(temp->visited_flag)) {
				int weight = temp->credit + courseList[courseID-1]->credit - cmin;
			
				if (weight<0)
					weight*=-(cmax-cmin+1);
				else if (weight>cmax-cmin)
					weight=std::numeric_limits<int>::max();
			
				if ( weight < leastConstraint) {
					leastConstraint = weight;
					leastSemester = temp;
				}
			}
			temp = temp->next;
		}
		
		leastSemester->visited_flag = true;
		semesterIDs.push_back(leastSemester->semesterID);
	}
	
	return semesterIDs;
}
