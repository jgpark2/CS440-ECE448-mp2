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
		currNode.courseList = courses;


		recursiveBack(currNode);


	return 0;
}



Gametree recursiveBack(GameState *cNode, GameTree *cTree)
{
	if (cNode.isValid())
		{
			if(cNode.isSolution()) return cNode; //if we have found the solution, return the gamestate
		}


	tempGameState = GameState(cNode);
	vector<Course*> tempCourses = cNode.courseList;
	bool valid = True;
	//select next variable (class)
	Course tryCourse;
	Course tryCourse = cNode.getMostConstrained(); //select most constrained variable, popped off of priority queue.

	int numValues = size(tryCourse.possibleSemeters) //find out how many semesters are possible = total number of classes
	for(int i =0; i<num_possible_semester; i++)
	{
		tempGameState.courseList[tryCourse].semesterID = i;
		if(tempGameState.isValid())
		{
			cNode.children.pushBack(tempGameState);
			tempGameState.parent = cNode;
			//TODO assign classes to semester in assignment map
			solution = recursiveBack(tempGameState);
			if(solution != false)
				return solution;
			else
			{
				delete tempGameState;
				cNode.children.pop_back();
			}

		}

	}



	return false; //all options failed











	


	}
}