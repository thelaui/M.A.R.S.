/* DropDownWindow.hpp

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

# ifndef DROPDOWNWINDOW_HPP_INCLUDED
# define DROPDOWNWINDOW_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

# include <vector>
# include <SFML/System.hpp>

class ComboBox;

class DropDownWindow: public UiWindow {
    public:
        DropDownWindow (int width, ComboBox* parent, std::vector<sf::String> elements);

        void checkWidgets();
        void onShow() {};

        void reset() {};

    private:
        ComboBox*                                  parent_;
        std::vector<std::pair<sf::String, bool> >  elements_;
};

# endif // DROPDOWNWINDOW_HPP_INCLUDED

