#include "gameState.h"
#include <iostream>       // std::cerr
#include <stdexcept>      // std::out_of_range
#include <vector>         // std::vector
#include <algorithm>    // std::find
#include <limits>
#include <stdio.h>
#include <queue>
#include <functional>

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
	childrenCount = 0;
	
	setCoursesFromVector(courses);
	
	//updateAssignment(); //Unnecessary if caller uses this only for an empty gamestate
	
	for(int i=0; i<=maxSemesterID; ++i)
		semesters.push_back(new Semester(i));
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
	
	
	for(int i=0; i<=maxSemesterID; ++i)
		semesters.push_back(new Semester(i));
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
			semesters[courseList[i]->semesterID]->addCourse(courseList[i]);
		}
	}
	
	//Loop again and fill in completely empty semesters with at least empty vector of course pointers
	//int semesterIDCount = 0;
	for(unsigned int i=0; i<assignment.size(); ++i) {
		assignment[i];
	}
}

//destructor
GameState::~GameState() {
	for (unsigned int i=0; i<courseList.size(); ++i) {
		delete courseList[i];
	}

	//Delete all semester...
	for(int i=0; i<=maxSemesterID; ++i)
		delete semesters[i];
	
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
	child->childrenCount = 0;
	GameState* counterProp = this;
	while(counterProp!=NULL) {
		counterProp->childrenCount++;
		counterProp = counterProp->parent;
	}
	
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
	int curSem = 0;
	
	//for each semester in the map
	for(map<int, vector<Course*> >::const_iterator it = assignment.begin(); it!=assignment.end(); ++it) {
		cout << "Semester: #" << it->first;
		if(it->first % 2 == 0)
			cout<< " FALL\t";
		else cout<<" SPRING\t";
		
		cout<<"("<<(it->second).size()<<" classes) Credits Taken: "<< semesterCredit(it->first)<<"\t";
		cout<<"Budget Change: "<<printBudget<<"->"<< (printBudget-semesters[curSem]->budget) << "(Used: " << semesters[curSem]->budget << ")"<<endl;
		printBudget -= semesters[curSem]->budget;
		
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
		curSem++;
	}
	
	
	/* FINAL PRINTS AS PRE MP2 SPECS */
	cout << "FINAL: " << endl;
	cout << totalBudget-printBudget << " " << curSem << endl;
	
	for(int i=0; i<curSem; i++) {
		cout << semesters[i]->courses.size() << " ";
		for (vector<Course*>::iterator it=semesters[i]->courses.begin(); it != semesters[i]->courses.end(); it++)
			cout << (*it)->courseID << " ";
		cout << endl;
	}
	
	for(int i=0; i<curSem; i++) {
		cout << semesters[i]->budget << " ";
	}
	cout << endl;
}

int GameState::mostConstrainedCourse() {
	//Reset RANK...
	/*for(unsigned int i=0; i<courseList.size(); ++i) {
		courseList[i]->constrained_rank = 0;
	}*/
	
	//Calculate RANK
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if(courseList[i]->interesting) {
			courseList[i]->constrained_rank += maxSemesterID;
			amplifyRank(courseList[i]->prereqList, 1);
		}
		else {
			amplifyRankMini(courseList[i]->prereqList, 0);
		}
	}
	
	//Find max rank...
	int courseID = -1;
	int maxReqRank = -1;
	
	for(unsigned int i=0; i<courseList.size(); ++i) {
		if (courseList[i]->semesterID!=-1)
			continue;
			
		if (courseList[i]->constrained_rank > maxReqRank) {
			courseID = i+1; //COURSEID STARTS AT 1
			maxReqRank = courseList[i]->constrained_rank;
		}
		//C CASE - this saves some nodes... ~328k -> ~276k, but the former gave loewr budget soln
		/*if(mode=='C' && courseList[i]->constrained_rank == maxReqRank &&
				( (courseList[i]->fallPrice < courseList[courseID-1]->fallPrice && 
					courseList[i]->fallPrice < courseList[courseID-1]->springPrice)
						|| 
					(courseList[i]->springPrice < courseList[courseID-1]->fallPrice && 
					courseList[i]->springPrice < courseList[courseID-1]->springPrice) )
			) {
			//Tie break with cheapest in both fall/spring field??
			courseID = i+1; //COURSEID STARTS AT 1
			maxReqRank = courseList[i]->constrained_rank;
		}*/
		
	}
	
	//if (courseID!=-1)////////////////////
	//	cout<<"MOST CONST: " << courseID << "   ";	////////////////
	
	return courseID;
}

void GameState::amplifyRank(vector<int> prereq, int depth) {
	if(prereq.size()==0)
		return;
		
	depth++;
	
	for(unsigned int i=0; i<prereq.size(); ++i) {
		courseList[prereq[i]-1]->constrained_rank += depth*maxSemesterID;
		amplifyRank(courseList[prereq[i]-1]->prereqList, depth);
	}
	
	return;
}

void GameState::amplifyRankMini(vector<int> prereq, int depth) {
	if(prereq.size()==0)
		return;
		
	depth++;
	
	for(unsigned int i=0; i<prereq.size(); ++i) {
		courseList[prereq[i]-1]->constrained_rank += depth;
		amplifyRankMini(courseList[prereq[i]-1]->prereqList, depth);
	}
	
	return;
}

vector<int> GameState::leastConstrainingValues(int courseID) {

	if (mode=='C') { // ||mode'B'
		//Return list of semesterIDs, sorted from least constraining to most constraining
		vector<int> semesterIDs;

		int spring = -1;
		if(courseList[courseID-1]->fallPrice < courseList[courseID-1]->springPrice)
			spring = 1;

		int beginIdx;
		spring>0? beginIdx=0:beginIdx=1;

		//CASE: REQUIRED COURSES
		if(courseList[courseID-1]->constrained_rank>=maxSemesterID) {
			//Lowest 2 SemIDs have priority, tie break for cheaper price.	
			for(int j=beginIdx; j<= maxSemesterID; j+=2) {
				if (semesters[j]->credit<cmax)
					semesterIDs.push_back(semesters[j]->semesterID);

				if(semesters[j+(1*spring)]->credit>=cmax)
					continue;

				semesterIDs.push_back(semesters[j+(1*spring)]->semesterID);
			}
			
			return semesterIDs;
		}
		
		int preferred_final_semester = semesters.back()->semesterID;
		//CASE: FILLERS
		for(int j=beginIdx; j<= maxSemesterID; j+=2) {
			//Lowest 2 SemIDs tie break for cheaper price iff former is filled to cmin
			if (semesters[j]->credit<cmin && j<=preferred_final_semester)
				semesterIDs.push_back(semesters[j]->semesterID);

			if (semesters[j+(1*spring)]->credit<cmin && j+(1*spring)<=preferred_final_semester)
				semesterIDs.push_back(semesters[j+(1*spring)]->semesterID);
		}
		
		return semesterIDs;
	}
	
	//MODE A
	/*for(int j=0; j<=maxSemesterID; ++j) {
		semesters[j]->visited_flag = false;
	}*/
	
	//Return list of semesterIDs, sorted from least constraining to most constraining
	vector<int> semesterIDs;
	
	//CASE: REQUIRED COURSES
	if(courseList[courseID-1]->constrained_rank>=maxSemesterID) {
		//Lowest SemID has priority, loop from 0~max
		for(int j=0; j<= maxSemesterID; ++j) {
			if (semesters[j]->credit>cmax)
				continue;
			semesterIDs.push_back(semesters[j]->semesterID);
		}
		
		return semesterIDs;
	}
	
	//CASE: filler courses
	//Lowest SemID has priority, loop from 0~max
	for(int j=0; j<= maxSemesterID; ++j) {
		if (semesters[j]->credit<cmin && semesters[j]->credit>0)
			semesterIDs.push_back(semesters[j]->semesterID);
	}
	
	return semesterIDs;
}
