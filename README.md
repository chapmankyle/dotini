<h1 align="center">dotini</h1>

<p align="center">
  <img src="https://circleci.com/gh/chapmankyle/dotini/tree/master.svg?style=svg" alt="Build Status"></img>
  <img src="https://img.shields.io/github/license/chapmankyle/dotini.svg?" alt="license: GPL-3.0"></img>
  <img src="https://img.shields.io/github/v/release/chapmankyle/dotini.svg?" alt="Release"></img>
</p>

A header-only C++ configuration file (`.ini`) parser :memo:

Built to favour readability over speed, for now, and stores all parsed key-value pairs inside a map for easy access.

# Setup :rocket:

Clone the repository and navigate to the `dotini` directory.
```bash
# clone the repo
git clone https://github.com/chapmankyle/dotini.git

# navigate to the `dotini` directory
cd dotini
```

# Usage :computer:

To use this in your project, simply copy the files inside the `src` directory and paste them into wherever you need a `.ini` file to be parsed :tada:

The `.ini` file reader is encapsulated in a class called **`INIReader`**, which takes the relative path to the `.ini` file as a parameter. 

## :unlock: Access Data

To access data from the `.ini` file, there are `get` methods for the data types `string`, `int`, `long`, `double` and `bool`.

These methods take in the following three parameters:
<ul>
  <li><code>section</code> &rarr; The name of the section in which the key-value pair is defined.</li>
  <li><code>key</code> &rarr; The key that you would like the value from.</li>
  <li><code>defVal</code> &rarr; The default value to return if the <code>section</code> or <code>key</code> is not found.</li>
</ul>

A full breakdown of the available methods and their descriptions can be seen below :boom:

| Method | Parameters | Return Value | Description |
| :----: | ---------- | ------------ | :---------- |
| `getString` | *`const std::string &section`*,<br/>*`const std::string &key`*,<br/>*`const std::string &defVal`* | **std::string** | Returns the value as a string |
| `getInt`    | *`const std::string &section`*,<br/>*`const std::string &key`*,<br/>*`const int defVal`*          | **int**         | Returns the value as an integer |
| `getLong`   | *`const std::string &section`*,<br/>*`const std::string &key`*,<br/>*`const long defVal`*         | **long**        | Returns the value as a long |
| `getDouble` | *`const std::string &section`*,<br/>*`const std::string &key`*,<br/>*`const double defVal`*       | **double**      | Returns the value as a double-precision floating-point number |
| `getBool`   | *`const std::string &section`*,<br/>*`const std::string &key`*,<br/>*`const bool defVal`*         | **bool**        | Returns the value as a boolean (`true` or `false`) |
| `getError`  | *`void`*                                                                                          | **std::string** | Returns a string representation of any error that occurred |
| `getSectionFields` | *`const std::string &section`*                                                             | **std::set&lt;Field&gt;** | Returns the fields associated with the given section |
| `getSectionNames`  | *`void`*                                                                                   | **std::set&lt;std::string&gt;** | Returns the names of the sections that were found in the `.ini` file |

## :bulb: Example
```C++
#include "ini.hpp"
#include <iostream>
#include <string>

int main() {
    // specify relative path to file
    INIReader reader("some_file.ini");
    
    // check for any errors during parsing
    if (!reader.success()) {
        std::cout << reader.getError() << '\n';
    }
    
    // get various data types from the file
    std::string str = reader.getString("SectionName", "Key", "");
    int i = reader.getInt("SectionName", "Key", 0);
    long l = reader.getLong("SectionName", "Key", 0L);
    double d = reader.getDouble("SectionName", "Key", 0.0);
    bool b = reader.getBool("SectionName", "Key", false);
    
    // show all section names and their associated fields
    for (const auto &section : reader.getSectionNames()) {
        std::cout << section << '\n';
        
        for (const auto &field : reader.getSectionFields(section)) {
            std::cout << "    " << field.toString() << '\n';
        }
    }
}
```
