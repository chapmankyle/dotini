#include "../src/ini.hpp"
#include <iostream>

int main() {

	// read from file
	INIReader reader("test/valid.ini");

	// check if any error occurred
	if (reader.success()) {
		std::cout << "Success!\n";
	} else {
		std::cout << "No success!\n";
	}

	return 0;
}
