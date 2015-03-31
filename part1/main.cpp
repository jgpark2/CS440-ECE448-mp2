#include <iostream>
#include <fstream>
#include <string>
//#include <stdlib.h>
#include <vector>
#include "course.h"
#include "semester.h"
#include "parse.h"
#include "gameTree.h"
#include "gameState.h"

using namespace std;
using namespace boost;

/*
 * Recursive Backtracking algorithm.
 * Returns NULL if no solution node is found for the CSP.
 */
GameState* recursiveBack(GameState* node)
{
	if(node->isSolution()) {
		return node;
	}
	
	//Select a variable to assign
	int courseID = node->mostConstrainedCourse();
	
	if(courseID==-1) //Although assign() catches it anyways...
		return NULL;

	//Try different values to assign to variable (semesterID starts at 0)
	vector<int> semesterIDCandidates = node->leastConstrainingValues(courseID);
	
	for(unsigned int i = 0; i < semesterIDCandidates.size(); ++i ) {
		GameState* assignment = node->assign(courseID, semesterIDCandidates[i]);
		//cout<<"assigning "<<courseID<<":"<<semesterIDCandidates[i]<<"...";//////////////////////////////////
		
		if (assignment==NULL) {//Probably exhausted all variables
			//cout<<";";//////////////
			return NULL;
		}
			
		GameState* result = NULL;
		if (assignment->isValid()) {
			//cout<<">";//////////////////////////////////
			result = recursiveBack(assignment);
		}
		
		if (result != NULL) {
			//cout<<"!";//////////////
			return result;
		}
	}

	//cout<<";";//////////////
	return NULL;
}

/*
 * Modifeid Recursive Backtracking algorithm. Lasts longer to try to find a minimal solution.
 * Returns NULL if no solution node is found for the CSP.
 * Return type only used for NULL or non-NULL checks... doesn't really matter
 */
GameState* recursiveBackNoStop(GameState* node, vector<GameState*>* solutions_ptr)
{
	if(node->isSolution()) {
		solutions_ptr->push_back(node);
		return node;
	}
	
	//Select a variable to assign
	int courseID = node->mostConstrainedCourse();
	
	if(courseID==-1) //Although assign() catches it anyways...
		return NULL;

	//Try different values to assign to variable (semesterID starts at 0)
	vector<int> semesterIDCandidates = node->leastConstrainingValues(courseID);
	
	for(unsigned int i = 0; i < semesterIDCandidates.size(); ++i ) {
		GameState* assignment = node->assign(courseID, semesterIDCandidates[i]);
		
		if (assignment==NULL) {//Probably exhausted all variables
			return NULL;
		}
			
		GameState* result = NULL;
		
		if (assignment->isValid()) {
			result = recursiveBackNoStop(assignment, solutions_ptr);
		}
		
		/*if (result != NULL) { Simply continues to finihs all semester assignments! I think there's some fwd checkign to reduce the domain of semesters here... if not, we can sure use it :/
			return result;
		}*/
	}
	
	return NULL;
}

int main(int argc, char* argv[])
{

	if (argc!=3) 
	{
		cout << "Error, usage: ./mp2 <task A,B,C> <scenario file>" << endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string scenario = argv[2];
	
	switch(mode)
	{
		case 'a':
		case 'A':
			mode = 'A';
			break;
			
		case 'b':
		case 'B':
			mode = 'B';
			break;
			
		case 'c':
		case 'C':
			mode = 'C';
			break;
			
		default:
			mode = 'A';
			cout << "Invalid task mode. Defaulting to A" << endl;
	}
	
	cout << "Task mode: " << mode << endl;

	Parse* p = new Parse();
	p->parseCourses(scenario);
	vector<Course*> courses = p->courses;
	
	int params_array[3];
	p->getParams(scenario, params_array);
	int Cmin = params_array[0];
	int Cmax = params_array[1];
	int budget = params_array[2];

	cout << "Cmin: " << Cmin;
	cout << " Cmax: " << Cmax;
	cout << " Budget: " << budget;
	cout << endl;

	GameTree* tree = new GameTree(new GameState(courses, Cmin, Cmax, budget, mode));
	
	/*if (mode=='C') {
		vector<GameState*> solutions;
		vector<GameState*>* solutions_ptr = &solutions;
		recursiveBackNoStop(tree->root, solutions_ptr);
		
		if(solutions.empty()) {
			cout << "No solutions found!!" << endl;
			return 0;
		}

		GameState* solution;
		int minimumBudget = budget+1;
		for(unsigned int i=0; i< solutions.size(); ++i) {
			int totalUsed = 0;
			for(unsigned int j=0; j<(solutions[i]->semesters).size(); j++) {
				totalUsed+=solutions[i]->semesters[j]->budget;
			}

			if(totalUsed<minimumBudget) {
				minimumBudget = totalUsed;
				solution = solutions[i];
			}
		}

		cout << "Solution found: " << endl;
		solution->printState();
		cout << "Nodes Expanded: " << tree->root->childrenCount+1 << endl;
	}
	else {*/
		GameState* solution = recursiveBack(tree->root);
		if (solution == NULL) {
			cout << "No solution found!!" << endl;
			return 0;
		}
		
		cout << "Solution found: " << endl;
		solution->printState();
		cout << "Nodes Expanded: " << tree->root->childrenCount+1 << endl;

		return 0;
	//}
}


