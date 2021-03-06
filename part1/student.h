#ifndef STUDENT_H
#define STUDENT_H

#include <vector>


class Student
{
	public:

		//default constructor
		Student();

		//param consstructor
		Student(int in_semester, int in_num_interesting_courses, std::vector<int> in_interesting_courses, int in_budget);

		//copy constructor
		Student(Student const &other);

		//destructor
		~Student();

	private:
		int semester;
		int num_interesting_courses;
		std::vector<int> interesting_courses;
		int budget;


};

#endif