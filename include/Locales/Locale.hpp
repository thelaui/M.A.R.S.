/* Locale.hpp

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

# ifndef LOCALE_HPP_INCLUDED
# define LOCALE_HPP_INCLUDED

# include <SFML/System.hpp>

struct Locale {
    Locale():
        name_("NULL"),
        fileName_("NULL"),
        font_("Ubuntu-R.ttf"),
        author_(""),
        LTR_(true) {}

    sf::String name_;
    sf::String fileName_;
    sf::String font_;
    sf::String author_;
    bool       LTR_;
};

# endif // LOCALE_HPP_INCLUDED
