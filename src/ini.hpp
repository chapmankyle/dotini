#pragma once

#ifndef INI_HPP
#define INI_HPP

#include <map>
#include <set>
#include <string>

#include "defines.hpp"

/**
 * @brief The different types of errors that may occur.
 */
enum class ErrorCode {
	None,
	NoSuchFile,
	NoClosingBracketForSection,
	EmptySection,
	NoValueForKey
};

/**
 * @brief Stores the key-value pair of a section entry in the file.
 */
struct Field {
	// Key used for lookup (case sensitive).
	std::string key;

	// Value associated with key.
	std::string value;

	/**
	 * @returns The key-value pair as a string in the form `key=value`.
	 */
	std::string toString() const {
		return key + '=' + value;
	}

	bool operator<(const Field &field) const {
		return key.compare(field.key) < 0;
	}

	bool operator==(const Field &field) const {
		return key.compare(field.key) == 0 && value.compare(field.value) == 0;
	}
};

/**
 * @brief Class that reads a `.ini` file and stores the name-value pairs
 * for easy access.
 */
class INIReader {

private:

	/**
	 * @brief The current line number in the file.
	 */
	int m_line_num;

	/**
	 * @brief Keeps track of whether or not the current line is inside a section.
	 */
	bool m_in_section{ false };

	/**
	 * @brief The current section (used when `m_in_section` is true).
	 */
	std::string m_curr_section;

	/**
	 * @brief Keep track of any error that occurs during parsing.
	 */
	ErrorCode m_error{ ErrorCode::None };

	/**
	 * @brief Lookup table containing key-values pairs, where the key is the
	 * name of the section and the values are the fields present in that section.
	 */
	std::map<std::string, std::set<Field>> m_lookup;

	/**
	 * @brief Names of all sections present in the given `.ini` file.
	 */
	std::set<std::string> m_section_names;

	/**
	 * @brief Removes any trailing whitespace (in place).
	 * @param str The string to remove trailing whitespace from.
	 */
	void rstrip(std::string &str);

	/**
	 * @returns `true` if there are sections with no key-value pairs, `false`
	 * otherwise.
	 */
	bool hasEmptySections();

	/**
	 * @brief Parses a section in a file.
	 * @param str The string containing the section.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parseSection(const std::string &str);

	/**
	 * @brief Parses a key inside a section in a file.
	 * @param str The string containing the key.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parseKey(const std::string &str);

	/**
	 * @brief Parses a section in a file.
	 * @param str The string containing the section.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parseValue(const std::string &str);

	/**
	 * @brief Parses a line of the file.
	 * @param str The current line in the file.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parseLine(const std::string &str);

	/**
	 * @brief Gets a string value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const std::string get(
		const std::string &section,
		const std::string &key,
		const std::string &defValue
	);

public:

	/**
	 * @brief Initializes the parser to read from a file.
	 * @param fileName The path of the file to read from.
	 */
	INIReader(const char *fileName);

	/**
	 * @brief Destructor for the parser.
	 */
	~INIReader() = default;

	/**
	 * @brief Check if initialization of INIReader was successful.
	 * @returns `true` if no error occurred parsing the file, `false` otherwise.
	 */
	const bool success() const;

	/**
	 * @brief Gets a string value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const std::string getString(
		const std::string &section,
		const std::string &key,
		const std::string &defValue
	);

	/**
	 * @brief Gets an integer value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const int getInt(
		const std::string &section,
		const std::string &key,
		int defValue
	);

	/**
	 * @brief Gets a long value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const long getLong(
		const std::string &section,
		const std::string &key,
		long defValue
	);

	/**
	 * @brief Gets a double-precision floating-point value from the
	 * configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const double getDouble(
		const std::string &section,
		const std::string &key,
		double defValue
	);

	/**
	 * @brief Gets a boolean value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	const bool getBool(
		const std::string &section,
		const std::string &key,
		bool defValue
	);

	/**
	 * @brief Gets the fields present in the given section.
	 * @param section The name of the section to get the fields from.
	 */
	std::set<Field> getSectionFields(const std::string &section) const {
		return m_lookup.at(section);
	}

	/**
	 * @returns The names of the sections present in the configuration file.
	 */
	std::set<std::string> getSectionNames() const {
		return m_section_names;
	}

};

#endif // !INI_HPP
