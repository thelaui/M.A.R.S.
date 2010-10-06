/* locales.cpp

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

# include "Locales/locales.hpp"

# include "System/settings.hpp"

# include <fstream>
# include <sstream>
# include <iostream>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>

namespace locales {

    namespace {
        std::vector<std::string> locales_(COUNT, "Error");
    }

    void load() {
        std::string fileName = "data/locales/"+settings::C_language+".txt";
        std::ifstream inStream(fileName.c_str());
        char line[1024];

        if (inStream.good()) {
            while(inStream.getline(line, 1024)) {
                std::string inputLine (line);

                while (inputLine[0] == ' ')
                    inputLine.erase(inputLine.begin());
                while (inputLine[inputLine.size()-2] == ' ')
                    inputLine.erase(inputLine.size()-2);

                // ignore comments and empty lines
                if(inputLine.size() == 2 || (inputLine.size() > 2 && (inputLine[0] != '/' && inputLine[1] != '/'))) {
                    std::istringstream iss (line);
                    int type;
                    iss >> type;
                    if (type < COUNT)
                        locales_[type] = std::string(iss.str(), 4);
                    else
                        std::cout << type << " is a bad ID in " << fileName <<"!\n";
                }
            }
        }
        else {
            std::cout << "Could not find " << fileName << "! Interface will be messed up with errors...\n";
        }
        inStream.close();
    }

    std::vector<std::string>const getLanguages() {
        DIR* dp;
        if((dp  = opendir("data/locales/")) == NULL)
            std::cout << "Error(" << errno << ") opening data/locales/" << std::endl;

        struct dirent* dirp;
        std::vector<std::string> languages;
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name[0] != '.') {
                std::string file(dirp->d_name);
                if (file.size() > 4)
                    languages.push_back(std::string(file, 0, file.size()-4));
                else
                    std::cout << "data/locales/" << file << " seems to be an invalid translation file!" << std::endl;
            }
        }
        closedir(dp);

        return languages;
    }

    std::string* getLocale(LocaleType type) {
        return &locales_[type];
    }
}


