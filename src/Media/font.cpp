/* font.cpp

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

# include "Media/font.hpp"

# include "System/settings.hpp"
# include "Locales/locales.hpp"

# include <iostream>

namespace font {
    namespace {
        sf::Font   *font_;

        void setFont(sf::String const& fileName) {
            if (!font_)
                font_ = new sf::Font();
            font_->LoadFromFile(settings::C_dataPath + "/fonts/" + fileName);
        }
    }

    sf::Font const& getFont() {
        if (font_ != NULL) {
            return *font_;
        }
        else {
            // load it from file and...
            setFont(*locales::getLocale(locales::FontName));
            // ... return it afterwards
            return *font_;
        }
    }

    void reload() {
        if (font_)
            delete font_;
        font_ = NULL;
        setFont(*locales::getLocale(locales::FontName));
    }
}
