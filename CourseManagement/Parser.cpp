/* Ahljenn Mallari
*ISO C++17 Standard (/std:c++17)*/

/* [ADD]:
* =====================================
*  Display Fall,Spring, or Summer courses only
*  # Of courses per subject code
*  # Of courses per particular professor
*  # Total "bad" courses (Assignment 14)
*  # Total courses (117)
*  # Total duplication
*  # Total courses in given semester
*  # Output all course in given semester
*  # Output amount of different term/sections an instructor has taught
*  # Output amount of different courses an instructor teaches
*
*  Multithreading possibility
*  Exception class handling
* 
*  Process total courses in private method
* 
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <utility>
#include <set>
#include <algorithm>
#include <thread>
#include <string>
#include <cstring>

struct course_info {

	friend std::ostream& operator<<(std::ostream&, course_info const&);

	std::string _term;
	std::string _section;
	std::string _course;
	std::string _instructor;
	std::string _time_location;
	std::string _subj_code;

};

class client {
public:
	client(const std::map<std::string, course_info>& processed_data) {
		_data = processed_data;
	}
	void show_menu();
	void interact();
	void display_all();
	void display_each();
	void display_crosslist();
	bool complete() const { return _state; };
private:
	std::map<std::string, course_info> _data;
	std::map<std::string, int> _subject_codes;
	int _total_courses{ 0 };
	bool _state{ false }; //completion state
	void process_subjects();
	const uint16_t W{ 10U }; //for outputting
};

void client::show_menu() {
	std::cout << "1. Display all courses and information.\n"
		<< "2. Display each courses and number of sections.\n"
		<< "3. Display 'cross-listed' courses.\n[Q] to quit\n\tInput: "; //ADD MORE OPTIONS later
	interact();
}

void client::interact() {
	std::string input;
	std::getline(std::cin, input);

	if (toupper(input[0]) == 'Q') { /*Client is finished*/
		_state = true;
		return;
	}

	switch (std::atoi(input.c_str())) {
	case 1: /*Display all*/
		display_all();
		break;
	case 2:
		display_each();
		break;
	case 3:
		break;
	default:
		std::cout << "Error\n";
	}
}

void client::display_all() {
	std::cout << "\n\n" << std::string(50, '=') << '\n';
	for (const auto& [k, v] : _data) { 
		std::cout << k << '\n' << std::string(50, '-') << '\n' << v << '\n';
	}
}

void client::display_each() {
	/*Pass function into private client method
	*Output after checking if database has been processed*/
	
	process_subjects();

	using namespace std::literals::chrono_literals;

	std::cout << std::left << std::setw(W) << "\nSUBJ-CODE" << std::right << std::setw(W + 13U) << "COUNT(s)\n";
	std::cout  << std::string(32, '=') << '\n';

	for (const auto& [k, v] : _subject_codes) {
		std::cout << std::left << std::setw(W) << k  << " " << std::right << std::setw(W) << v << " section(s)\n";
		std::this_thread::sleep_for(0.05s);
	}
	std::cout << std::string(32, '-') << "\nTotal number of subjects: " << _subject_codes.size() << "\n\n";
}

void client::display_crosslist() /*Assignm. 14*/ {

}

void client::process_subjects() /*Only process the subject codes once*/ {
	/* Check to see if the subject codes have already been processed
	* If processed, return
	* Else, process only process once.*/
	if (_subject_codes.empty()) { 
		for (const auto& [k, v] : _data) {
			_subject_codes[v._subj_code]++;
		}
	}
	else {
		return;
	}
}

std::ostream& operator<<(std::ostream& os, course_info const& rhs) {
	os << "Course: " << rhs._course << '\t' << rhs._subj_code << '\n'
		<< "Instructor: " << rhs._instructor << '\n'
		<< "When/Where: " << rhs._time_location << "\n\n";
	return os;
}

void parser(std::ifstream& in_file, std::map<std::string, course_info>& data) { //Refine this function later on

	char* token;
	char buff[1000];
	const char* const tab{ "," };

	while (in_file.good()) {

		std::string line;
		std::getline(in_file, line);
		std::strcpy(buff, line.c_str());
		if (buff[0] == 0) continue; //skip blanks
		const std::string term{ token = strtok(buff,tab) };
		const std::string section{ (token = strtok(0,tab)) ? token : "" };
		const std::string course{ (token = strtok(0,tab)) ? token : "" };
		const std::string instructor{ (token = strtok(0, tab)) ? token : "" };
		const std::string when_where{ (token = strtok(0, tab)) ? token : "" };
		if (course.find('-') == std::string::npos) continue; //not found. no dash in course name
		const std::string subj_code{ course.begin(), course.begin() + course.find('-') };

		course_info temp{ term, section, course, instructor, when_where, subj_code };
		data[term + ' ' + section] = temp;
	}
}

int main() {

	std::ifstream in_file("dvc-schedule.csv");
	if (!in_file.is_open()) {
		throw std::runtime_error("Cannot open.");
	}

	std::map<std::string, course_info> course_data;
	//[k] = term/section
	//[v] = course information

	std::cout << "This is a course management system, press [ENTER] to continue:\n"; //introduction while parsing data
	std::thread worker(parser,std::ref(in_file),std::ref(course_data)); 
	std::cin.get();
	worker.join();

	client local_client(course_data); //pass map by const ref
	while (!local_client.complete()){
		local_client.show_menu();
	}
}