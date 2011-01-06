/* Checkbox.hpp

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

# ifndef CHECKBOX_HPP_INCLUDED
# define CHECKBOX_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

class Checkbox: public UiElement {
    public:
        Checkbox (sf::String* text, bool* value, Vector2f const& topLeft, int width);
        ~Checkbox ();

        void mouseMoved(Vector2f const& position);
        void mouseLeft(bool down);
        void keyEvent(bool down, sf::Key::Code keyCode);

        void draw() const;

        void setFocus  (UiElement* toBeFocused);
        void clearFocus();

    private:
        bool* value_;
        Label* label_;
};

# endif

