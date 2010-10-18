/* font.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include "Media/font.hpp"

# include <iostream>

namespace font {
    namespace {
        std::vector<sf::Font*> fonts_(COUNT);

        void loadFont_(FontType type, std::string fileName) {
            fonts_[type] = new sf::Font;
            fonts_[type]->LoadFromFile(fileName);
        }
    }

    sf::Font const& getFont(FontType type) {
        if (fonts_[type] != NULL) {
            return *fonts_[type];
        }
        else {
            // load it from file and...
            switch (type) {
                case HandelGotDLig:  loadFont_(type, "data/fonts/HandelGotDLig.ttf");   break;
                case FreeSans:       loadFont_(type, "data/fonts/FreeSans.ttf");        break;

                case COUNT: std::cout << "COUNT is not a valid Texturetype..." << std::endl;
            }
            // ... return it afterwards
            return *fonts_[type];
        }
    }
}
