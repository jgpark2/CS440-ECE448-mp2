#include <iostream>
#include <fstream>
#include <string>
//#include <stdlib.h>
#include <vector>
#include "course.h"
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
	for(int semesterID = 0; semesterID <= node->maxSemester; ++semesterID ) {
		GameState* assignment = node->assign(courseID, semesterID);
		//cout<<"assigning "<<courseID<<":"<<semesterID<<"...";//////////////////////////////////
		
		if (assignment==NULL) //Probably exhausted all variables
			return NULL;
			
		GameState* result = NULL;
		if (assignment->isValid()) {
			//cout<<" ...was valid";//////////////////////////////////
			result = recursiveBack(assignment);
		}
		
		if (result != NULL)
			return result;
	}

	return NULL;
}


int main(int argc, char* argv[])
{

	if (argc!=3) 
	{
		cout << "Error, usage: ./mp2 [search flag] [scenario file name]" << endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string scenario = argv[2];
	
	switch(mode)
	{
		default:
			cout << "Search flag mode: " << mode << endl;
			break;
	}

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

	GameTree* tree = new GameTree(new GameState(courses, Cmin, Cmax, budget));
	
	GameState* solution = recursiveBack(tree->root);
	if (solution == NULL) {
		cout << "No solution found!!" << endl;
		return 0;
	}
	
	cout << "Solution found: " << endl;
	solution->printState();

	return 0;
}


