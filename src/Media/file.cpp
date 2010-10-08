/* file.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# include "Media/file.hpp"

# include "Media/utf8.hpp"

# include <iostream>
# include <fstream>
# include <sstream>

namespace file {
    std::vector<sf::String> const load(std::string fileName) {
        std::vector<sf::String> strings;
        // Open the test file (contains UTF-8 encoded text)
        std::ifstream fileStream(fileName.c_str());
        if (!fileStream.is_open()) {
            std::cout << "Could not open " << fileName << std::endl;
        }
        else {
            std::string line;
            int lineCount(0);
            // Play with all the lines in the file
            while (std::getline(fileStream, line)) {
                ++ lineCount;
                // remove '\r' at end of lines, when file has a CR LF EOL (windows...)
                if (*line.rbegin() == '\r') line.erase(line.end()-1);
                // check for invalid utf-8 (for a simple yes/no check, there is also utf8::is_valid function)
                std::string::iterator end_it = utf8::find_invalid(line.begin(), line.end());
                if (end_it != line.end()) {
                    std::cout << "Invalid UTF-8 encoding detected at line " << lineCount << "!\n";
                    std::cout << "This part is fine: " << std::string(line.begin(), end_it) << "\n";
                }
                // Convert it to utf-32
                std::basic_string<sf::Uint32> utf32line;
                utf8::utf8to32(line.begin(), line.end(), back_inserter(utf32line));

                // ignore comments and nearly empty lines
                if(utf32line.size() > 2 && (utf32line[0] != 47 && utf32line[1] != 47))
                    strings.push_back(sf::String(utf32line));
            }
            fileStream.close();
        }
        return strings;
    }
}

