#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <map>

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
		
		void evaluateConstrainedRank();
		
		int courseID;
		int fallPrice;
		int springPrice;
		int credit;
		int semesterID;
		std::vector <int> prereqList; //Contains COURSEID's (1 based)
		std::vector <int> is_prereq_for; //Contains COURSEID's (1 based)
		bool interesting;
		int constrained_rank;
		
		//map of semesterID's (value domain) to bool (assignable or no longer possible)
		std::map <int, bool> reaminingValues;
};

#endif
