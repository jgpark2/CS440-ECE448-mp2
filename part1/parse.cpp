#include "parse.h"

using namespace std;
using namespace boost;

void Parse::getParams(string scenario, int (&array)[3] )
{
	string curr_line;
	int Cmin;
	int Cmax;
	int num_courses;
	int budget;

	ifstream myfile (scenario.c_str());

	if(!myfile.is_open())
	{
		cout << "Error opening file" << endl;
		return;
	}
	if(getline(myfile, curr_line) == NULL)
	{
		cout << "Error: no lines to read" << endl;
		//return 1;
		return;
	}

	istringstream(curr_line) >> num_courses >> Cmin >> Cmax;
	array[0] = Cmin;
	array[1] = Cmax;
	// cout << "Number of courses: " << num_courses << endl;
	// cout << "Cmin: " << Cmin << endl;
	// cout << "Cmax: " << Cmax << endl;
	for(int i = 0; i < (num_courses*2)+1; i++)
	{
		getline(myfile, curr_line);
	}
	
	//get budget
	getline(myfile, curr_line);
	budget = atoi(curr_line.c_str());
	array[2] = budget;

	return;
}

void Parse::parseCourses(string scenario)
{
		//for parsing the scenario in general
		string curr_line;
		unsigned int num_courses;
		int Cmin;
		int Cmax;

		//list of courses
		//vector<Course> courses;
		//vector<Course*> course_ptrs;

		//for parsing each course
		int fall_price;
		int spring_price;
		int credit_hours;

		//for parsing prereqs
		vector<int> curr_reqs;
		vector<string> fields;
		int curr_req;

		//for parsing interesting courses
		vector<int> interesting_courses;
		string curr_interesting_course;

		ifstream myfile (scenario.c_str());
	
		//failure to open file
		if(!myfile.is_open()) 
		{
			cout << "Error opening file" <<endl;
			//return 1;
			return;// courses;
		}

		if(getline(myfile, curr_line) == NULL)
		{
			cout << "Error: no lines to read" << endl;
			//return 1;
			return;// courses;
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
			/*Course* curr_course = new Course (i+1, fall_price, spring_price, credit_hours);
			courses.push_back(*curr_course);*/
			courses.push_back(new Course (i+1, fall_price, spring_price, credit_hours));
		}

		//populates course pointers array course_ptrs
		/*for(unsigned int i = 0; i < num_courses; i++)
		{
			course_ptrs.push_back(&courses[i]);
		}*/

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
				//courses[i]->prereqList.push_back(courses[curr_req-1]);
				courses[i]->prereqList.push_back(curr_req);
				courses[curr_req-1]->is_prereq_for.push_back(i);
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
			courses[interesting_courses[i]-1]->interesting = true;
		}

		//forward checking - populates legal semester assignments for each course's legal_semesters (a vector of ints)
		for(unsigned int i = 0; i < num_courses; i++)
		{
			//queries number of prereqs s.t. legal_assignment's least value in legal_semesters is the number of prereqs (1 prereq, least value is 1)
			unsigned int least_legal_value = courses[i]->prereqList.size();

			for(unsigned int j = least_legal_value; j < courses.size(); j++)
			{
				courses[i]->legal_semesters.push_back(j);
			}
		}


		for(unsigned int i = 0; i < num_courses; i++)
		{
			cout << "Course " << courses[i]->courseID;
			cout << "\tFall price: " << courses[i]->fallPrice;
			cout << "\tSpring price: " << courses[i]->springPrice;
			cout << "\tCredits: " << courses[i]->credit;
			cout << "\tPrerequisites: ";
			for(unsigned int j = 0; j < courses[i]->prereqList.size(); j++)
			{
				cout << courses[i]->prereqList[j] << ", ";
			}
			cout << "\tIs prereq for: ";
			for(unsigned int j = 0; j < courses[i]->is_prereq_for.size(); j++)
			{
				int prereq_index = courses[i]->is_prereq_for[j];
				Course* temp_ptr = courses[prereq_index];
				cout << temp_ptr->courseID << ", ";
			}
			cout << "\tInteresting flag: ";
			if(courses[i]->interesting==true)
			{
				cout << "true";
			}
			else
			{
				cout << "false";
			}
			cout <<"\tLegal Semester assignments: " << courses[i]->legal_semesters[0] << " through " << courses[i]->legal_semesters[(courses[i]->legal_semesters.size())-1] << endl;
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
		return;// courses;
}
