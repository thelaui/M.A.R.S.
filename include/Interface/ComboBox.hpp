/* ComboBox.hpp

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

# ifndef COMBOBOX_HPP_INCLUDED
# define COMBOBOX_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

class UiWindow;

class ComboBox: public UiElement {
    public:
        ComboBox (sf::String* text, sf::String* value, std::vector<sf::String> values, Vector2f const& topLeft, int width, int labelWidth=185);
        ~ComboBox ();

        void mouseLeft(bool down);

        void draw() const;

        friend class DropDownWindow;

    private:
        UiWindow* dropBox_;

        sf::String* currentValue_;
        Label* label_;
        int labelWidth_;
        bool opened_;
};

# endif // COMBOBOX_HPP_INCLUDED

