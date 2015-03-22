#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <vector>
#include "course.h"

using namespace std;
using namespace boost;

class Parse{
	public:
	vector<Course*> courses;

	void parseCourses(string scenario);
	void getParams(string scenario, int (&array)[3] );

};

#endif
