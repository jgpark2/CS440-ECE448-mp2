#include "course.h"

using namespace std;

//default constructor
Course::Course() {
	courseID = 0;
	fallPrice = 0; //free
	springPrice = 0; //
	credit = 0; //no hours
}

//param constructor
Course::Course(int courseID, int fallPrice, int springPrice, int creditHours)
{
	this->courseID = courseID;
	this->fallPrice = fallPrice;
	this->springPrice = springPrice;
	this->credit = creditHours;
}

//copy constructor
Course::Course(Course const &other)
{
	courseID = other.courseID;
	fallPrice = other.fallPrice;
	springPrice = other.springPrice;
	credit = other.credit;
	prereqList = other.prereqList;
}

//destructor
Course::~Course()
{
	/*for(int i=0; i<prereqList.size(); ++i) {
		delete prereqList[i];
	}*/
}
