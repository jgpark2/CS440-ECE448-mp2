#ifndef COURSE_H
#define COURSE_H

#include <vector>

class Course
{
	public:

		//default constructor
		Course();

		//param consstructor
		Course(int in_fall_price, int in_spring_price, int in_credit_hours);

		//copy constructor
		Course(Course const &other);

		//destructor
		~Course();

		int get_fall_price();
		int get_spring_price();
		int get_credit_hours();
		
	private:
		int fall_price;
		int spring_price;
		int credit_hours;
};

#endif