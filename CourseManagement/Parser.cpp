/* Ahljenn Mallari
*ISO C++17 Standard (/std:c++17)*/

/* [ADD]:
*  =====================================
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
*  Be able to search professor by name, similar to final project using .find()
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <unordered_map>
#include <utility>
#include <set>
#include <algorithm>
#include <thread>
#include <string>
#include <cstring>

const std::string FILE_NAME{ "dvc-schedule.csv" };

struct course_info {

	friend std::ostream& operator<<(std::ostream&, course_info const&);
	std::string _term;
	std::string _section;
	std::string _course;
	std::string _instructor;
	std::string _time_location;
	std::string _subj_code;
};

std::ostream& operator<<(std::ostream& os, course_info const& rhs) {
	os << "Course: " << rhs._course << '\t' << rhs._subj_code << '\n'
		<< "Instructor: " << rhs._instructor << '\n'
		<< "When/Where: " << rhs._time_location << "\n\n";
	return os;
}

class client {
public:
	client(std::unordered_map<std::string, course_info>& data, 
		std::unordered_map<std::string, std::set<std::string>>& invalid) {
		_data = std::move(data); //move resources to this client
		_course_check = std::move(invalid);
		process_subjects();
	}
	void show_menu();
	bool complete() const { return _state; }; //completion state

private:
	void interact();
	void display_all();
	void display_each();
	void display_totals();
	void display_instructors();
	void find_instructor();
	void display_invalid();
	void process_subjects();

	int _total_courses{ 0 };
	bool _state{ false }; 
	const uint16_t W{ 10U }; 
	const std::size_t S{ 50U };

	std::unordered_map<std::string, course_info> _data;
	std::unordered_map<std::string, std::set<std::string>> _instructors;
	std::unordered_map<std::string, std::set<std::string>> _course_check;
	std::map<std::string, int> _subject_codes; //keep this sorted
};

void client::show_menu() {
	std::cout << '\n' << std::string(S, '-') <<"\nClient menu:\n" << std::string(S, '=');
	std::cout << "\n1. Display all courses and information.\n"
		<< "2. Display each courses and number of sections.\n"
		<< "3. Display totals.\n"
		<< "4. Find an instructor\n"
		<< "5. Display all instructors.\n"
		<< "6. Display invalid courses.";
	std::cout << '\n' << std::string(S, '=')
		<< "\n[Q] to quit\n\tInput: ";
	interact();
}

void client::interact() {
	std::string input;
	std::getline(std::cin, input);

	if (toupper(input[0]) == 'Q') {
		_state = true;
		return;
	}

	switch (std::atoi(input.c_str())) {
	case 1: 
		display_all();
		break;
	case 2:
		display_each();
		break;
	case 3:
		display_totals();
		break;
	case 4:
		find_instructor();
		break;
	case 5:
		display_instructors();
		break;
	case 6:
		display_invalid();
		break;
	default:
		std::cout << "Error, try again!\n";
		break;
	}
}

void client::display_all() {
	std::cout << "\n\n" << std::string(50, '=') << '\n';
	for (const auto& [k, v] : _data) { 
		std::cout << k << '\n' << std::string(50, '-') << '\n' << v << '\n';
	}
}

void client::display_each() {
	using namespace std::literals::chrono_literals;

	std::cout << "\n\n" << std::string(50, '=') << '\n';
	std::cout << std::left << std::setw(W) << "\nSUBJ-CODE & COUNT(s)\n";
	std::cout  << std::string(32, '=') << '\n';

	for (const auto& [k, v] : _subject_codes) {
		std::cout << std::left << std::setw(W) << k  << " " << std::right << std::setw(W) << v << " section(s)\n";
		std::this_thread::sleep_for(0.05s);
	}
	std::cout << std::string(32, '-') << "\nTotal number of subjects: " << _subject_codes.size() << "\n\n";
}

void client::display_totals() {
	std::cout << "\n\n" << std::string(50, '=') << '\n';
	std::cout << "Total valid courses: " << _data.size()
		<< "\nTotal different subjects: " << _subject_codes.size() << '\n';
}

void client::display_instructors() {
	std::cout << "\n\n" << std::string(50, '=') << '\n';

	for (const auto& [k,v] : _instructors) {
		int counter{ 0 };
		std::cout <<"\nProfessor: " << k << '\n' << std::string(30, '=') << '\n';
		for (const auto& course_itr : v) {
			std::cout << course_itr << '\n';
			++counter;
		}
		std::cout << std::string(30, '-') << "\nTotal classes taught: " << counter << '\n';
	}
}

void client::find_instructor() {
	std::string input;

	while (input.length() == 0) {
		std::cout << "Please search name for an instructor: ";
		std::getline(std::cin, input);
	}

	//Format name
	for (std::size_t i{ 0 }; i < input.length(); ++i) {
		input[i] = tolower(input[i]);
	}
	input[0] = toupper(input[0]);

	if (_instructors.find(input) != _instructors.cend()) {
		std::cout << "\nCourses found for " << input << "(s):\n" << std::string(30, '-') << '\n';
		for (const auto i : _instructors[input]) {
			std::cout << i << '\n';
		}
		std::cout << "\nImportant note: results may be outputting additional results due to some instructors with the same name.\n";
	}
	else {
		std::cout << "\nCourses could not be found for " << input << ".\n";
	}
}

void client::display_invalid(){
	std::cout << "\n\n" << std::string(50, '=') << '\n';

	int counts{ 1 };
	for (const auto& i : _course_check) {
		if (i.second.size() > 1) {
			std::cout << "Invalid Term/Section:\n" << std::string(20, '-') << '\n';
			std::cout << counts << ". " << i.first << '\n' << std::string(20, '=') << '\n';
			for (const auto& j : i.second) {
				std::cout << "Course: " << j << '\n';
			}
			std::cout << "\n\n";
			++counts;
		}
	}
	std::cout << "There are a total of " << _course_check.size() << " invalid term section pairs.\n\n";
}

void client::process_subjects(){

	/* This thread handles the main map of courses
	 * Process counts for each subject code
	 * Process courses for each instructor, adding: course, section, term.*/
	std::thread worker([this](const std::unordered_map<std::string, course_info>& dat) {
		for (const auto& [k, v] : dat) {
			_subject_codes[v._subj_code]++;
			_instructors[v._instructor].insert(v._course + '-' + v._section + ": " + v._term);
	}}, _data);
	
	/*This thread handles the secondary map (of sets) for invalid courses.
	 *This process will copy the sets with nonduplicates with size greater than 1 
	 *[Note: Set already deduplicates]*/
	std::thread worker2([this](std::unordered_map<std::string, std::set<std::string>>&& dat) {
		std::unordered_map<std::string, std::set<std::string>> temp;
		for (const auto& i : dat) {
			if (i.second.size() > 1) {
				for (const auto& j : i.second) {
					temp[i.first].insert(j);
				}
			}
		}
		_course_check.clear();
		_course_check = std::move(temp);
	},  _course_check);

	worker.join(); 
	worker2.join(); 
}

void parser(std::ifstream& in_file, std::unordered_map<std::string, course_info>& data, 
	std::unordered_map<std::string, std::set<std::string>>& invalids) { 

	/*Term and section must be unique
	 *Copies information into a map of courses (Key: Term & Section)
	 *Copies information into a map of sets    (Key: Term & Section)*/

	char* token;
	char buff[1000];
	const char* const tab{ "," };
	int lines{ 0 };

	std::cout << "\nParsing database: [";
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
		++lines;

		course_info temp{ term, section, course, instructor, when_where, subj_code };

		data[term + ' ' + section] = temp;
		invalids[term + ' ' + section].insert(course); //add to our map

		if (lines % 5000 == 1) {
			std::cout << '.';
			std::cout.flush();
		}
	}
	std::cout << "]\nProcess complete.\nPress [ENTER] to continue: \n";
}

int main() {

	std::ifstream in_file(FILE_NAME);
	if (!in_file.is_open()) {
		throw std::runtime_error("Cannot open.");
	}

	std::unordered_map<std::string, course_info> course_data;
	/*[k] = term & section
	  [v] = course information*/

	std::unordered_map<std::string, std::set<std::string>> invalid_course;
	/*[k] = term & section
	  [v] = course*/

	std::cout << "This is a course management system...\n"; //introduction while parsing data
	std::thread parse(parser,std::ref(in_file),std::ref(course_data), std::ref(invalid_course)); 
	std::cin.get();
	parse.join();

	client local_client(course_data,invalid_course);

	while (!local_client.complete()) {
		local_client.show_menu();
	}
}