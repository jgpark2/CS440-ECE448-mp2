#ifndef SEMESTER_H
#define SEMESTER_H

#include <vector>
#include "course.h"

/*
 * Semester class, unreliable since parallel data must be kept consistent with assignment map in gameState.
 */
class Semester
{
	public:		
		//default constructor
		Semester(int semesterID, Semester* prev, Semester* next);
		
		std::vector<Course*> courses;
		int semesterID;
		Semester* prev;
		Semester* next;
		int budget; //Current budget USED after signing up for *this* semester
		int credit; //Current credit taken
		
		bool visited_flag;
		
		void addCourse(Course* course);
};

#endif
