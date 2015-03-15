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
		int courseID;
		int fallPrice;
		int springPrice;
		int credit;
		vector<Course*> prereqList;
		
		//default constructor
		Course();

		//param constructor
		Course(int fallPrice, int springPrice, int credit);

		//copy constructor
		Course(Course const &other);

		//destructor
		~Course();
		
};

#endif
