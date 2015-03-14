#ifndef STUDENT_H
#define STUDENT_H

#include <vector>

using namespace std;

class student
{
	public:

		//default constructor
		Student();

		//param consstructor
		Student(int in_semester, int in_num_interesting_courses, vector in_interesting_courses, int in_budget);

		//copy constructor
		Student(Student const &other);

		//destructor
		~Student();

	private:
		int semester;
		vector<int> interesting_courses;
		int budget;


};

#endif