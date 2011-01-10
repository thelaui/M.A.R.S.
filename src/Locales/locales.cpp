/* locales.cpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

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
# include "Media/file.hpp"

# include <sstream>
# include <iostream>
# include <sys/types.h>
# include <dirent.h>
# include <errno.h>

namespace locales {

    namespace {
        std::vector<sf::String> locales_(COUNT, "Error");

        void load(std::string const& fileName) {
            std::vector<sf::String> lines;
            if (file::load(fileName, lines)) {
                for (std::vector<sf::String>::iterator it = lines.begin(); it != lines.end(); ++it) {
                    std::stringstream sstr(it->ToAnsiString());
                    int id;
                    sstr >> id;
                    if (id < COUNT && it->GetSize() > 4) {
                        sf::String tmp(*it);
                        tmp.Erase(0, 4);
                        locales_[id] = tmp;
                    }
                }
            }
            else {
                std::cout << "Failed to open locale " << fileName << "! Interface will be messed up with errors...\n";
            }
        }
    }

    void load() {
        load (settings::C_dataPath + "/locales/English.txt");
        load (settings::C_dataPath + "/locales/"+settings::C_language+".txt");
    }

    std::vector<sf::String>const getLanguages() {
        DIR* dp;
        if((dp  = opendir((settings::C_dataPath + "/locales/").c_str())) == NULL)
            std::cout << "Error(" << errno << ") opening data/locales/" << std::endl;

        struct dirent* dirp;
        std::set<sf::String> languageSet;
        while ((dirp = readdir(dp)) != NULL) {
            if (dirp->d_name[0] != '.') {
                std::string file(dirp->d_name);
                if (file.size() > 4)
                    languageSet.insert(sf::String(std::string(file, 0, file.size()-4)));
                else
                    std::cout << settings::C_dataPath + "/locales/" << file << " seems to be an invalid translation file!" << std::endl;
            }
        }
        closedir(dp);

        std::vector<sf::String> languages(languageSet.begin(), languageSet.end());


        return languages;
    }

    sf::String* getLocale(LocaleType type) {
        return &locales_[type];
    }
}


