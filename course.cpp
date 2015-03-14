#include "course.h"

using namespace std;

//default constructor
Course::Course()
{
	fall_price = 0; //free
	spring_price = 0;
	credit_hours = 0; //no hours
}

//param constructor
Course::Course(int in_fall_price, int in_spring_price, int in_credit_hours)
{
	fall_price = in_fall_price; //free
	spring_price = in_spring_price;
	credit_hours = in_credit_hours; //no hours
}

//copy constructor
Course::Course(Course const &other)
{
	fall_price = other.fall_price; //free
	spring_price = other.spring_price;
	credit_hours = other.credit_hours; //no hours
}

//destructor
Course::~Course()
{
	//
}

//get fall price
int Course::get_fall_price()
{
	return this->fall_price;
}

int Course::get_spring_price()
{
	return this->spring_price;
}

int Course::get_credit_hours()
{
	return this->credit_hours;
}