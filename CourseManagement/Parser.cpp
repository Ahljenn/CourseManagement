/* Ahljenn Mallari
*ISO C++17 Standard (/std:c++17)*/

/* [ADD]:
* =====================================
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
*  Multithreading possibility?
*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <map>
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
	client(const std::map<std::string, course_info>& arg) {
		data = arg;
	}
	void display_all();
private:
	std::map<std::string, course_info> data;
};

void client::display_all() {
	using namespace std::literals::chrono_literals;
	for (const auto& [k, v] : data) { //output all
		std::cout << k << '\n' << std::string(50, '-') << '\n' << v << '\n';
		std::this_thread::sleep_for(0.5s);
	}
}


std::ostream& operator<<(std::ostream& os, course_info const& rhs) {
	os << "Course: " << rhs._course << '\t' << rhs._subj_code << '\n'
		<< "Instructor: " << rhs._instructor << '\n'
		<< "When/Where: " << rhs._time_location << "\n\n";
	return os;
}

void parser(std::ifstream& in_file, std::map<std::string, course_info>& data) {

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

	using namespace std::literals::chrono_literals;
	std::map<std::string, course_info> course_data;
	//[k] = term/section
	//[v] = course information

	std::cout << "This is a course management system, press [ENTER] to continue:\n";
	std::thread worker(parser,std::ref(in_file),std::ref(course_data)); 
	std::cin.get();
	worker.join();

	client local_client(course_data); //pass map by const ref

	
}