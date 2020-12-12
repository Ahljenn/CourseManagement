/* Ahljenn Mallari
*ISO C++17 Standard (/std:c++17)*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include <cstring>

struct course_info {

	friend std::ostream& operator<<(std::ostream&, course_info const&);

	std::string _course;
	std::string _instructor;
	std::string _time_location;
};

std::ostream& operator<<(std::ostream& os, course_info const& rhs) {
	os << "Course: " << rhs._course << '\n'
		<< "Instructor: " << rhs._instructor << '\n'
		<< "When/Where: " << rhs._time_location << "\n\n";
	return os;
}

int main() {

	std::ifstream in_file("dvc-schedule.csv");

	if (!in_file.is_open()) {
		throw std::runtime_error("Cannot open.");
	}

	std::map<std::string, course_info> course_data;

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
		const std::string subjCode{ course.begin(), course.begin() + course.find('-') };
		
		course_info temp{ course,instructor,when_where };
		course_data[term + ' ' + section] = temp;
	}

	for (const auto& [k, v] : course_data) {
		std::cout << k << '\n' << std::string(50, '-') << '\n' << v << '\n';
	}
}