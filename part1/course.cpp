#include "course.h"

using namespace std;

//default constructor
Course::Course() {
	courseID = 0;
	fallPrice = 0; //free
	springPrice = 0; //
	credit = 0; //no hours
	semesterID = -1;
	//is_prereq_for = vector<int>();
	interesting = false;
	constrained_rank = -1;
}

//param constructor
Course::Course(int courseID, int fallPrice, int springPrice, int creditHours)
{
	this->courseID = courseID;
	this->fallPrice = fallPrice;
	this->springPrice = springPrice;
	this->credit = creditHours;
	this->semesterID=-1;

	//TODO: double check
	this->is_prereq_for = vector<int>();
	this->prereqList = vector<int>();
	this->interesting = false;
	this->constrained_rank = -1;
}

//copy constructor
Course::Course(Course const &other)
{
	courseID = other.courseID;
	fallPrice = other.fallPrice;
	springPrice = other.springPrice;
	credit = other.credit;
	prereqList = other.prereqList;
	semesterID = other.semesterID;
	is_prereq_for = other.is_prereq_for;
	interesting = other.interesting;
	constrained_rank = other.constrained_rank;
}

//destructor
Course::~Course()
{
}
