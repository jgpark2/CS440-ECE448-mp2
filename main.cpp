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

	vector<Course*> courses = parseCourses(scenario);
	int params_array[3];
	getParams(scenario, params_array);
	int Cmin = params_array[0];
	int Cmax = params_array[1];
	int budget = params_array[2];

	GameTree tree;
	GameState currNode(courses, Cmin, Cmax, budget);
	currNode.printState();

	return 0;
}
