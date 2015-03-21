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

		//for parsing interesting courses
		vector<int>interesting_courses;
		string curr_interesting_course;

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

		//populates course array courses
		for(unsigned int i = 0; i < num_courses; i++)
		{
			getline(myfile, curr_line);
			istringstream(curr_line) >> fall_price >> spring_price >> credit_hours;
			
			//kept safe in memory VERY IMPORTANT TO HAVE "new"!
			//note that there is NO SUCH THING AS COURSE 0 (if there was, then course 9 would be a pre-req of itself in firstScenario.txt)
			Course* curr_course = new Course (i+1, fall_price, spring_price, credit_hours);
			courses.push_back(*curr_course);
		}

		//populates course pointers array course_ptrs
		for(unsigned int i = 0; i < num_courses; i++)
		{
			course_ptrs.push_back(&courses[i]);
		}

		for(unsigned int i = 0; i < num_courses; i++)
		{
			getline(myfile, curr_line);
			//referenced: http://www.cplusplus.com/faq/sequences/strings/split/
			split( fields, curr_line, is_any_of(" "), token_compress_on);
			for(unsigned int j = 1; j < fields.size(); j++)
			{
				//j is the course whose prereqs are listed in fields[1 through fields.size()-1]
				string curr_string = fields[j];
				//convert string to integer
				//referenced: https://stackoverflow.com/questions/7663709/convert-string-to-int-c
				curr_req = atoi(curr_string.c_str());
				//push back pointer to prereqList
				courses[i].prereqList.push_back(&courses[curr_req-1]);
				courses[curr_req-1].is_prereq_for.push_back(i);
			}
		}

		//gets list of interesting courses
		getline(myfile, curr_line);
		for(string::iterator it=curr_line.begin(); it!=curr_line.end(); ++it)
		{
			//if we're still reading from a valid number, i.e. we do not reach a delimiter
			if( *(it)!=' ')
			{
				//gets the current character and append it to curr_interesting_course
				curr_interesting_course.push_back(*it); //push back the char digit

				//for last char digit
				string::iterator it_test = it;
				it_test++;
				if(it_test == curr_line.end())
				{
					//cout << "last char: " << *(it) << endl;
					interesting_courses.push_back(atoi(curr_interesting_course.c_str()));
					//clear the string for the next iteration
					curr_interesting_course.clear();
				}
			}
			//if we reach a delimiter (space in our case)
			else
			{
				//convert curr_interesting course string into an integer and push it onto interesting_courses int vector
				//cout << "Adding interesting course: " << curr_interesting_course.c_str() << endl;
				interesting_courses.push_back(atoi(curr_interesting_course.c_str()));
				//clear the string for the next iteration
				curr_interesting_course.clear();
			}
		}

		//assign respective interesting flag in each course
		for(unsigned int i = 1; i < interesting_courses.size(); i++)
		{
			courses[interesting_courses[i]-1].interesting = true;
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
			cout << " Is prereq for: ";
			for(unsigned int j = 0; j < course_ptrs[i]->is_prereq_for.size(); j++)
			{
				int prereq_index = course_ptrs[i]->is_prereq_for[j];
				Course* temp_ptr = course_ptrs[prereq_index];
				cout << temp_ptr->courseID << ", ";
			}
			cout << "Interesting flag: ";
			if(course_ptrs[i]->interesting==true)
			{
				cout << "true";
			}
			else
			{
				cout << "false";
			}
			cout << endl;
		}
		cout << "There are " << interesting_courses[0] << " interesting courses: ";
		for(unsigned int j = 1; j < interesting_courses.size(); j++)
		{
			cout << interesting_courses[j] << ", ";
		}
		cout << endl;

		cout << "End of parse" << endl;

		myfile.close();
		return course_ptrs;
}