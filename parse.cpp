#include "parse.h"

using namespace std;
using namespace boost;

vector<Course*> parseCourses(string scenario)
{
		//for parsing the scenario in general
		string curr_line;
		unsigned int num_courses;
		int Cmin;
		int Cmax;

		//list of courses
		vector<Course> courses;
		vector<Course*> course_ptrs;

		//for parsing each course
		int fall_price;
		int spring_price;
		int credit_hours;

		//for parsing prereqs
		vector<int> curr_reqs;
		vector<string> fields;
		int curr_req;

		ifstream myfile (scenario.c_str());
	
		//failure to open file
		if(!myfile.is_open()) 
		{
			cout << "Error opening file" <<endl;
			//return 1;
			return course_ptrs;
		}

		if(getline(myfile, curr_line) == NULL)
		{
			cout << "Error: no lines to read" << endl;
			//return 1;
			return course_ptrs;
		}

		//the first string is already stored in curr_line
		//istringstream stores space delimited numbers in string to respective variables
		//referenced: https://stackoverflow.com/questions/10130432/parsing-space-delimited-numbers-from-getline
		istringstream(curr_line) >> num_courses >> Cmin >> Cmax;

		cout << "Number of courses: " << num_courses << endl;
		cout << "Cmin: " << Cmin << endl;
		cout << "Cmax: " << Cmax << endl;

		// switch(mode)
		// {
		// 	default:
		// 		cout << "Search flag mode: " << mode << endl;
		// 		break;
		// }

		for(unsigned int i = 0; i < num_courses; i++)
		{
			getline(myfile, curr_line);
			istringstream(curr_line) >> fall_price >> spring_price >> credit_hours;
			Course curr_course(i, fall_price, spring_price, credit_hours);
			courses.push_back(curr_course);
		}

		for(unsigned int i = 0; i < num_courses; i++)
		{
			getline(myfile, curr_line);
			//referenced: http://www.cplusplus.com/faq/sequences/strings/split/
			split( fields, curr_line, is_any_of(" "), token_compress_on);
			for(unsigned int j = 1; j < fields.size(); j++)
			{
				string curr_string = fields[j];
				//convert string to integer
				//referenced: https://stackoverflow.com/questions/7663709/convert-string-to-int-c
				curr_req = atoi(curr_string.c_str());
				//push back pointer to prereqList
				courses[i].prereqList.push_back(&courses[curr_req]);
			}
		}

		for(unsigned int i = 0; i < num_courses; i++)
		{
			course_ptrs.push_back(&courses[i]);
		}

		for(unsigned int i = 0; i < num_courses; i++)
		{
			cout << "Course " << course_ptrs[i]->courseID;
			cout << " Fall price: " << course_ptrs[i]->fallPrice;
			cout << " Spring price: " << course_ptrs[i]->springPrice;
			cout << " Credits: " << course_ptrs[i]->credit;
			cout << " Prerequisites: ";
			for(unsigned int j = 0; j < course_ptrs[i]->prereqList.size(); j++)
			{
				cout << course_ptrs[i]->prereqList[j]->courseID << ", ";
			}
			cout << endl;
		}
		cout << "End of parse" << endl;

		return course_ptrs;
}