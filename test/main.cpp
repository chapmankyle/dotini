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

	std::string title = reader.getString("WINDOW", "Title", "");

	double fov = reader.getDouble("GRAPHICS", "FOV", 0.0);
	bool vsync = reader.getBool("GRAPHICS", "VSYNC", false);

	double masterVol = reader.getDouble("AUDIO", "Master", 0.0);
	double backgroundVol = reader.getDouble("AUDIO", "Background", 0.0);
	double subtitleVol = reader.getDouble("AUDIO", "Subtitle", 0.0);

	std::cout << "Window title: " << title << '\n';
	std::cout << '\n';
	std::cout << "Field of view: " << fov << '\n';
	std::cout << "Vsync: " << std::boolalpha << vsync << std::noboolalpha << '\n';
	std::cout << '\n';
	std::cout << "Master volume: " << masterVol << '\n';
	std::cout << "Background volume: " << backgroundVol << '\n';
	std::cout << "Subtitle volume: " << subtitleVol << '\n';

	std::cout << "\nSections present in file:\n";
	for (const auto &sec : reader.getSectionNames()) {
		std::cout << sec << '\n';

		for (const auto &field : reader.getSectionFields(sec)) {
			std::cout << "    " << field.toString() << '\n';
		}
	}

	return 0;
}
