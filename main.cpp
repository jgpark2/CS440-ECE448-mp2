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
	(tree->root->assign(1,1))->printState();

	return 0;
}
