#pragma once

#ifndef INI_HPP
#define INI_HPP

#include <map>
#include <set>
#include <string>

#ifndef ALLOW_MULTILINE
#	define ALLOW_MULTILINE 1
#endif

#ifndef ALLOW_COMMENTS
#	define ALLOW_COMMENTS 1
#endif

#ifndef ALLOW_INLINE_COMMENTS
#	define ALLOW_INLINE_COMMENTS 1
#endif

#ifndef STOP_ON_FIRST_ERROR
#	define STOP_ON_FIRST_ERROR 0
#endif

#if ALLOW_COMMENTS
#	ifndef START_COMMENT_PREFIXES
#		define START_COMMENT_PREFIXES ";#"
#	endif
#endif

#if ALLOW_INLINE_COMMENTS
#	ifndef INLINE_COMMENT_PREFIXES
#		define INLINE_COMMENT_PREFIXES ";"
#	endif
#endif

#define MAX_SECTION_LENGTH 50
#define MAX_KEY_LENGTH     50
#define MAX_LINE_LENGTH    200

/**
 * @brief The different types of errors that may occur.
 */
enum class ErrorCode {
	None,
	NoSuchFile,
	NoClosingBracketForSection,
	EmptySection,
	KeyOutsideSection,
	NoValueForKey,
	NoClosingQuotationForValue
};

/**
 * @brief String representations of the errors that can occur.
 */
static const std::string errorStrings[]{
	"No error has occurred.",
	"File does not exist.",
	"No closing bracket found for section.",
	"Section has no key-value pairs.",
	"Key-value pair was found outside a section.",
	"No value found for key.",
	"No closing double quotes for value."
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
	 * @brief Removes any leading whitespace (in place).
	 * @param str The string to remove leading whitespace from.
	 */
	void lstrip(std::string &str);

	/**
	 * @brief Removes any trailing whitespace (in place).
	 * @param str The string to remove trailing whitespace from.
	 */
	void rstrip(std::string &str);

	/**
	 * @brief Removes both leading and trailing whitespace (in place).
	 * @param str The string to remove leading and trailing whitespace from.
	 */
	void trim(std::string &str);

	/**
	 * @returns `true` if there are sections with no key-value pairs, `false`
	 * otherwise.
	 */
	bool hasEmptySections();

	/**
	 * @brief Removes comments from a string (in place).
	 */
	void removeComment(std::string &str);

	/**
	 * @brief Parses a section in a file.
	 * @param str The string containing the section.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parseSection(const std::string &str);

	/**
	 * @brief Parses a key-value pair inside a section in a file.
	 * @param k The string containing the key.
	 * @param v The string containing the value.
	 * @returns `true` if no errors occurred, `false` otherwise.
	 */
	bool parsePair(const std::string &k, const std::string &v);

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
	) const;

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
	) const;

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
		const int defValue
	) const;

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
		const long defValue
	) const;

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
		const double defValue
	) const;

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
		const bool defValue
	) const;

	/**
	 * @returns A string representation of the error that occurred.
	 */
	const std::string getError() const {
		return errorStrings[static_cast<int>(m_error)];
	}

	/**
	 * @brief Gets the fields present in the given section.
	 * @param section The name of the section to get the fields from.
	 */
	const std::set<Field> getSectionFields(const std::string &section) const {
		return m_lookup.at(section);
	}

	/**
	 * @returns The names of the sections present in the configuration file.
	 */
	const std::set<std::string> getSectionNames() const {
		return m_section_names;
	}

};

#endif // !INI_HPP
