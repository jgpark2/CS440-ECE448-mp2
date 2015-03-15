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

	return 0;
}
