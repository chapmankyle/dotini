#include "ini.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <regex>

void INIReader::lstrip(std::string &str) {
	str = std::regex_replace(str, std::regex("^ +"), "");
}


void INIReader::rstrip(std::string &str) {
	str = std::regex_replace(str, std::regex(" +$"), "");
}


void INIReader::trim(std::string &str) {
	lstrip(str);
	rstrip(str);
}


bool INIReader::hasEmptySections() {
	// no empty sections if map is empty
	if (m_lookup.empty()) {
		return false;
	}

	// check if each section has at least one key-value pair
	for (const auto &entry : m_lookup) {
		if (entry.second.empty()) {
			return true;
		}
	}

	return false;
}


void INIReader::removeComment(std::string &str) {
	const std::string s(START_COMMENT_PREFIXES);
	char commentChar;
	std::size_t commentIdx;

	bool commentFound{ false };
	for (const auto &c : s) {
		commentIdx = str.find(c);

		if (commentIdx != std::string::npos) {
			commentFound = true;
			commentChar = c;
			break;
		}
	}

	// no comment in string
	if (!commentFound) {
		return;
	}

	// found comment
	str = str.substr(0, commentIdx);
	rstrip(str);
}


bool INIReader::parseSection(const std::string &str) {
	// before parsing each section, check that at least one key-value pair is
	// found in the previous section
	if (hasEmptySections()) {
		m_error = ErrorCode::EmptySection;
		return false;
	}

	m_in_section = true;
	std::size_t closingIdx = str.find(']');

	// could not find closing square bracket
	if (closingIdx == std::string::npos) {
		m_error = ErrorCode::NoClosingBracketForSection;
		return false;
	}

	// get name of section, removing any trailing whitespace inside section declaration
	std::string sec = str.substr(1, closingIdx - 1);
	rstrip(sec);
	m_curr_section = sec;

	// add section to names of sections
	m_section_names.insert(sec);
	return true;
}


bool INIReader::parsePair(const std::string &k, const std::string &v) {
	// no key-value pair allowed outside section
	if (!m_in_section) {
		m_error = ErrorCode::KeyOutsideSection;
		return false;
	}

	// copy initialize to not affect original string
	std::string key = k;
	std::string val = v;

	// strip all whitespace
	trim(key);
	trim(val);

	if (val.empty()) {
		m_error = ErrorCode::NoValueForKey;
		return false;
	}

	// value is a string
	if (val.at(0) == '"') {
		const std::size_t endIdx = val.rfind('"');

		if (endIdx == std::string::npos || endIdx == 0) {
			m_error = ErrorCode::NoClosingQuotationForValue;
			return false;
		}

		val = val.substr(1, endIdx - 1);
		rstrip(val);
	} else {
		removeComment(val);
	}

	// create field
	Field nextField;
	nextField.key = key;
	nextField.value = val;

	// look for existing section
	std::map<std::string, std::set<Field>>::iterator found = m_lookup.find(m_curr_section);

	// section does not exist
	if (found == m_lookup.end()) {
		// create new set to hold all fields
		std::set<Field> fields;
		fields.insert(nextField);

		// add field to map
		m_lookup.insert(std::map<std::string, std::set<Field>>::value_type(m_curr_section, fields));
		return true;
	}

	// section already exists, insert new field
	found->second.insert(nextField);
	return true;
}


bool INIReader::parseLine(const std::string &str) {
	// ignore newlines
	if (str.length() < 1) {
		return true;
	}

	const std::string s(START_COMMENT_PREFIXES);

	// ignore start-of-line comments
	if (s.find(str.at(0)) != std::string::npos) {
		return true;
	}

	// start of section
	if (str.at(0) == '[') {
		return parseSection(str);
	}

	const std::size_t assignIdx = str.find('=');

	// check if assignment operator (=) exists
	if (assignIdx == std::string::npos) {
		m_error = ErrorCode::NoValueForKey;
		return false;
	}

	return parsePair(
		str.substr(0, assignIdx),                            // key
		str.substr(assignIdx + 1, str.length() - assignIdx)  // value
	);
}


const std::string INIReader::get(
	const std::string &section,
	const std::string &key,
	const std::string &defValue
) const {
	// no section exists
	if (m_lookup.find(section) == m_lookup.end()) {
		return defValue;
	}

	// find value associated with key
	for (const auto &field : m_lookup.at(section)) {
		if (field.key == key) {
			return field.value;
		}
	}

	return defValue;
}


INIReader::INIReader(const char *fileName) {
	// read file
	std::ifstream file(fileName);

	// check if file could be opened
	if (file.fail()) {
		m_error = ErrorCode::NoSuchFile;
		return;
	}

	std::string currLine;
	m_line_num = 1;

	// go through each line in file
	while (std::getline(file, currLine)) {
		// strip trailing whitespace
		rstrip(currLine);

		// parse current line
		if (!parseLine(currLine)) {
			break;
		}

		++m_line_num;
	}

	// close file reader
	file.close();
}


const bool INIReader::success() const {
	return m_error == ErrorCode::None;
}


const std::string INIReader::getString(
	const std::string &section,
	const std::string &key,
	const std::string &defValue
) const {
	const std::string str = get(section, key, "");
	return str.empty() ? defValue : str;
}


const int INIReader::getInt(
	const std::string &section,
	const std::string &key,
	int defValue
) const {
	const std::string str = get(section, key, "");
	return str.empty() ? defValue : std::stoi(str);
}


const long INIReader::getLong(
	const std::string &section,
	const std::string &key,
	long defValue
) const {
	const std::string str = get(section, key, "");
	return str.empty() ? defValue : std::stol(str);
}


const double INIReader::getDouble(
	const std::string &section,
	const std::string &key,
	double defValue
) const {
	const std::string str = get(section, key, "");
	return str.empty() ? defValue : std::stod(str);
}


const bool INIReader::getBool(
	const std::string &section,
	const std::string &key,
	bool defValue
) const {
	std::string str = get(section, key, "");
	if (str.empty()) {
		return defValue;
	}

	// convert to lowercase for case-insensitive checking
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		[](unsigned char c){ return std::tolower(c); }
	);

	if (str == "true" || str == "yes" || str == "on" || str == "1") {
		return true;
	} else if (str == "false" || str == "no" || str == "off" || str == "0") {
		return false;
	} else {
		return defValue;
	}
}
