#include "semester.h"
#include <iostream> 

using namespace std;

//default constructor
Semester::Semester(int semesterID, Semester* prev, Semester* next) {
	(this->courses).clear();
	this->semesterID = semesterID;
	this->prev = prev;
	this->next = next;
	budget = 0;
	credit = 0; //Current credit taken
	visited_flag = false;
}

void Semester::addCourse(Course* course) {
	if (course->semesterID != semesterID)
		cout << "wth?" << endl;
	
	courses.push_back(course);
	
	if(semesterID%2==0)
		budget+=course->fallPrice;
	else budget+=course->springPrice;
	credit+=course->credit;
}
