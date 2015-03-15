#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "course.h"

using namespace std;

int main(int argc, char* argv[])
{

	if (argc!=3) 
	{
		cout << "Error, usage: ./mp2 [search flag] [scenario file name]" << endl;
		return 1;
	}
	
	char mode = argv[1][0];
	string m_size = argv[2];
	
	ifstream myfile (m_size.c_str());
	
	//failure to open file
	if(!myfile.is_open()) 
	{
		cout << "Error opening file" <<endl;
		
		return 1;
	}
	//file is opened
	else
	{
		string curr_line;
		int num_courses;
		int Cmin;
		int Cmax;
		vector<Course> courses;
		int fall_price;
		int spring_price;
		int credit_hours;

		if(getline(myfile, curr_line) == NULL)
		{
			cout << "Error: no lines to read" << endl;
			return 1;
		}

		//the first string is already stored in curr_line
		//istringstream stores space delimited numbers in string to respective variables
		//referenced: https://stackoverflow.com/questions/10130432/parsing-space-delimited-numbers-from-getline
		istringstream(curr_line) >> num_courses >> Cmin >> Cmax;

		cout << "Number of courses: " << num_courses << endl;
		cout << "Cmin: " << Cmin << endl;
		cout << "Cmax: " << Cmax << endl;

		switch(mode)
		{
			default:
				cout << "Search flag mode: " << mode << endl;
				break;
		}

		for(int i = 0; i < num_courses; i++)
		{
			getline(myfile, curr_line);
			istringstream(curr_line) >> fall_price >> spring_price >> credit_hours;
			Course curr_course(fall_price, spring_price, credit_hours);
			courses.push_back(curr_course);
		}

		for(int i = 0; i < num_courses; i++)
		{
			cout << "Course " << courses[i].courseID;
			cout << " Fall price: " << courses[i].fallPrice;
			cout << " Spring price: " << courses[i].springPrice;
			cout << " Credits: " << courses[i].credit << endl;
		}

		cout << "End of parse" << endl;

		return 0;
	}

	return 0;
}
