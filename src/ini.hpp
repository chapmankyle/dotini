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
	 * @brief Keep track of any error that occurs during parsing.
	 */
	ErrorCode m_error{ ErrorCode::None };

	/**
	 * @brief Lookup table containing key-values pairs, where the key is the
	 * name of the section and the values are the fields present in that section.
	 */
	std::map<const char *, std::set<Field>> m_lookup;

	/**
	 * @brief Names of all sections present in the given `.ini` file.
	 */
	std::set<const char *> m_section_names;

	/**
	 * @brief Removes any trailing whitespace (in place).
	 * @param str The string to remove trailing whitespace from.
	 */
	void rstrip(std::string &str);

	/**
	 * @brief Parses a line of the file.
	 * @param str The current line in the file.
	 */
	void parseLine(const std::string &str);

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
	const char *get(const char *section, const char *key, const char *defValue);

	/**
	 * @brief Gets an integer value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	int getInt(const char *section, const char *key, int defValue);

	/**
	 * @brief Gets a long value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	long getLong(const char *section, const char *key, long defValue);

	/**
	 * @brief Gets a double-precision floating-point value from the
	 * configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	double getDouble(const char *section, const char *key, double defValue);

	/**
	 * @brief Gets a boolean value from the configuration file.
	 * @param section The name of the section to get the value from.
	 * @param key The key associated with the value.
	 * @param defValue The value to return if no such section or key is found.
	 * @returns The associated value if `key` is found, else `defValue`.
	 */
	bool getBool(const char *section, const char *key, bool defValue);

	/**
	 * @brief Gets the fields present in the given section.
	 * @param section The name of the section to get the fields from.
	 */
	std::set<Field> getSectionFields(const char *section) const {
		return m_lookup.at(section);
	}

	/**
	 * @returns The names of the sections present in the configuration file.
	 */
	std::set<const char *> getSectionNames() const {
		return m_section_names;
	}

};

#endif // !INI_HPP
