/* Key.hpp

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

# ifndef KEY_HPP_INCLUDED
# define KEY_HPP_INCLUDED

# include <SFML/Window.hpp>

struct Key {
    Key(sf::Key::Code code);
    Key(unsigned int joyID, sf::Joy::Axis joyAxis, int strength);
    Key(unsigned int joyID, unsigned int joyButton);

    enum KeyType  {kKeyBoard, kJoyButton, kJoyAxis} type_;
    enum NavType  {nAbort, nConfirm, nDown, nUp, nLeft, nRight, nNone} navi_;
    enum AxisType {aLT, aRT, aARup, aARdown, aARleft, aARright, aALup, aALdown,
                   aALleft, aALright, aPOVup, aPOVdown, aPOVleft, aPOVright};

    union {
        sf::Key::Code keyBoard_;
        AxisType      joyAxis_;
        unsigned int  joyButton_;
    } code_;

    int strength_;
    unsigned int joyID_;

    static std::pair<AxisType, int> convertFromSFML(sf::Joy::Axis joyAxis, int strength);
    static sf::Joy::Axis convertToSFML(AxisType joyAxis);
};

bool operator== (Key const& lhs, Key const& rhs);
std::ostream& operator<< (std::ostream& stream, Key const& input);
std::istream& operator>> (std::istream& stream, Key& output);

# endif // KEY_HPP_INCLUDED
