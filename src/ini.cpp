#include "ini.hpp"

#include <iostream>
#include <fstream>
#include <regex>

void INIReader::rstrip(std::string &str) {
	str = std::regex_replace(str, std::regex(" +$"), "");
}


INIReader::INIReader(const char *fileName) {
	// read file
	std::ifstream file(fileName);

	// check if file could be opened
	if (file.fail()) {
		std::cerr << "Could not read file\n";
		return;
	}

	std::string currLine;
	int lineNum = 1;

	// go through each line in file
	while (std::getline(file, currLine)) {
		std::cout << '[' << lineNum << "]  " << currLine << '\n';
		++lineNum;
	}

	// close file reader
	file.close();
}


const char* INIReader::get(const char *section, const char *key, const char *defValue) {
	return "";
}


int INIReader::getInt(const char *section, const char *key, int defValue) {
	return 0;
}


long INIReader::getLong(const char *section, const char *key, long defValue) {
	return 0L;
}


double INIReader::getDouble(const char *section, const char *key, double defValue) {
	return 0.0;
}


bool INIReader::getBool(const char *section, const char *key, bool defValue) {
	return true;
}
