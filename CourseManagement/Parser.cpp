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
	client(std::unordered_map<std::string, course_info>& data) {
		_data = std::move(data); //move resources to this client
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
	void process_subjects();

	int _total_courses{ 0 };
	bool _state{ false }; 
	const uint16_t W{ 10U }; 
	const std::size_t S{ 50U };

	std::unordered_map<std::string, course_info> _data;
	std::unordered_map<std::string, std::set<std::string>> _instructors;
	std::map<std::string, int> _subject_codes; //keep this sorted
};

void client::show_menu() {
	std::cout << '\n' << std::string(S, '-') <<"\nClient menu:\n" << std::string(S, '=');
	std::cout << "\n1. Display all courses and information.\n"
		<< "2. Display each courses and number of sections.\n"
		<< "3. Display totals.\n"
		<< "4. Find an instructor\n"
		<< "5. Display all instructors.";
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

	std::cout << std::left << std::setw(W) << "\nSUBJ-CODE & COUNT(s)\n";
	std::cout  << std::string(32, '=') << '\n';

	for (const auto& [k, v] : _subject_codes) {
		std::cout << std::left << std::setw(W) << k  << " " << std::right << std::setw(W) << v << " section(s)\n";
		std::this_thread::sleep_for(0.05s);
	}
	std::cout << std::string(32, '-') << "\nTotal number of subjects: " << _subject_codes.size() << "\n\n";
}

void client::display_totals() {
	std::cout << '\n' << std::string(50, '=') << '\n';
	std::cout << "Total valid courses: " << _data.size()
		<< "\nTotal different subjects: " << _subject_codes.size() << '\n';
}

void client::display_instructors() {

	std::cout << "\n\n" << std::string(50, '=');

	for (const auto& [k,v] : _instructors) {
		int counter{ 0 };
		std::cout << "\nProfessor: " << k << '\n' << std::string(30, '-') << '\n';
		for (const auto& course_itr : v) {
			std::cout << course_itr << '\n';
			++counter;
		}
		std::cout << "Total classes taught: " << counter << '\n';
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
		std::cout << "\nImportant note: results may be outputting additional results because some instructors with the same name.\n";
	}
	else {
		std::cout << "\nCourses could not be found for " << input << ".\n";
	}
}

void client::process_subjects(){
#if 0
	std::thread worker([this](const std::unordered_map<std::string, course_info>& dat) {
		for (const auto& [k, v] : dat) {
			_subject_codes[v._subj_code]++;
		}}, _data);
	std::thread worker2([this](const std::unordered_map<std::string, course_info>& dat) {
		for (const auto& [k, v] : dat) {
			_instructors[v._instructor].insert(v._course + '-' + v._section + ": " + v._term);
		}}, _data);
	worker.join();
	worker2.join();
#endif
	for (const auto& [k, v] : _data) {
		_subject_codes[v._subj_code]++;
		_instructors[v._instructor].insert(v._course + '-' + v._section + ": " + v._term);
	}
}

void parser(std::ifstream& in_file, std::unordered_map<std::string, course_info>& data) { //Refine this function later on
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

	std::unordered_map<std::string, course_info> course_data;
	//[k] = term/section
	//[v] = course information

	std::cout << "This is a course management system...\nPress [ENTER] to continue:"; //introduction while parsing data
	std::thread parse(parser,std::ref(in_file),std::ref(course_data)); 
	std::cin.get();
	parse.join();

	client local_client(course_data); //pass unordered_map by const ref
	while (!local_client.complete()) {
		local_client.show_menu();
	}
}