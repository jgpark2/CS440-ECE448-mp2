#include "student.h"

//default constructor
Student::Student()
{
	semester = 0; //fall semester (index 0)
	num_interesting_courses = 0; //no interesting courses
	vector<int> temp_vector; //empty vector for interesting_courses
	interesting_courses = temp_vector;
	budget = 0; //no budget
}

//param constructor
Student::Student(int in_semester, int in_num_interesting_courses, vector in_interesting_courses, int in_budget)
{
	semester = in_semester;
	num_interesting_courses = in_num_interesting_courses;
	interesting_courses = in_interesting_courses;
	budget = in_budget;
}

Student::Student(Student const &other)
{
	semester = other.in_semester;
	num_interesting_courses = other.in_num_interesting_courses;
	interesting_courses = other.in_interesting_courses;
	budget = other.in_budget;
}

//destructor
Student::~Student()
{
	//
}