/* LabeledBox.hpp

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

# ifndef LABELEDBOX_HPP_INCLUDED
# define LABELEDBOX_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "System/Color3f.hpp"

class Label;

class LabeledBox: public UiElement {
    public:
        LabeledBox(sf::String* text, Vector2f const& topLeft, int width, int height);

        ~LabeledBox();

        void draw() const;

        bool isTabable() const {return false;}

    private:
        Label* label_;
};

# endif //LABELEDBOX_HPP_INCLUDED

