#include <iostream>
#include <fstream>
#include <string>
//#include <stdlib.h>
#include <vector>
#include "course.h"
#include "parse.h"

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
	
	vector<Course*> courses = parseCourses(scenario);

	switch(mode)
	{
		default:
			cout << "Search flag mode: " << mode << endl;
			break;
	}

		GameTree tree = new GameTree(); //creates new tree for student
		GameState currNode = tree.root; //define current Node to start at root
		schedule = 

		recursiveBack(currNode, tree);


	return 0;
}



Gametree recursiveBack(GameState *cNode, GameTree *cTree)
{
	if (cNode.isSolution()) return cNode;
	else
	{
		int numChild = 
		for(int i = 0; i<)
		Course tryCourse = cNode.children[i];
		if(tryCourse.isValid())
		{
			recursiveBack(tryCourse, cTree)
		}

	if(tryCourse.isValid())
		{
			schedule.pushBack( new Course( *courses[i]))

		}

	}


	


	}
}