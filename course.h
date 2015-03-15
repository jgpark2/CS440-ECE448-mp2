#ifndef COURSE_H
#define COURSE_H

#include <vector>

/*
 * Course class, these courses have unique IDs that can be mapped 1-to-1 to array indices.
 * So make a vector of courses to reference all courses. There are helper functions though.
 */
class Course
{
	public:		
		//default constructor
		Course();

		//param constructor
		Course(int courseID, int fallPrice, int springPrice, int credit);

		//copy constructor
		Course(Course const &other);

		//destructor
		~Course();
		
		int courseID;
		int fallPrice;
		int springPrice;
		int credit;
		int semesterID;
		std::vector <Course*> prereqList;
};

#endif
