#include "../src/ini.hpp"
#include <iostream>

int main() {

	// read from file
	INIReader reader("test/valid.ini");

	// check if any error occurred
	if (!reader.success()) {
		std::cout << "No success!\n";
		std::cout << reader.getError() << '\n';
		return 1;
	}

	std::cout << '\n';
	for (const auto &sec : reader.getSectionNames()) {
		std::cout << sec << '\n';
	}

	return 0;
}
