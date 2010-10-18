/* Label.hpp

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

# ifndef LABEL_HPP_INCLUDED
# define LABEL_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "System/Color3f.hpp"
# include "Hud/hud.hpp"

class Label: public UiElement {
    public:
        Label (sf::String* text, int textAlign, Vector2f const& topLeft, float fontSize = 12.f, Color3f color = Color3f(1,1,1));

        void mouseLeft(bool down) {}

        void draw() const;

    private:
        sf::String* text_;
        int textAlign_;
        float fontSize_;
        Color3f color_;
};

# endif
