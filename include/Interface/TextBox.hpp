/* TextBox.hpp

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

# ifndef TEXTBOX_HPP_INCLUDED
# define TEXTBOX_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

# include <vector>

class TextBox: public UiElement {
    public:
        TextBox (sf::String* text, Vector2f const& topLeft, int width, int height);
        ~TextBox();

        void mouseMoved(Vector2f const& position) {}
        void mouseLeft(bool down) {}

        void draw () const;
        void setText(sf::String* text);

    private:
        sf::String text_;
        Label* label_;
};

# endif




