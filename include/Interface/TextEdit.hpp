/* TextEdit.hpp

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

# ifndef TEXTEDIT_HPP_INCLUDED
# define TEXTEDIT_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

# define TEXT_EDIT  0
# define PORT_EDIT  1
# define IP_EDIT    2

class TextEdit: public UiElement {
    public:
        TextEdit (sf::String* text, sf::String* value, Vector2f const& topLeft, int width, int labelWidth, int type, int maxLength=0);
        ~TextEdit ();

        void mouseMoved(Vector2f const& position);
        void mouseLeft(bool down);
        void keyEvent(bool down, Key const& key);
        void textEntered(int keyCode);

        void draw() const;

        void setFocus  (UiElement* toBeFocused, bool isPrevious);
        void clearFocus();

    private:
        sf::String* value_;
        Label* label_;
        unsigned maxLength_;
        unsigned cursorPos_;
        mutable int cursorTimer_;
        int type_;
        int labelWidth_;
};

# endif
