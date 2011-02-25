/* VerticalSlider.hpp

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

# ifndef VERTICALSLIDER_HPP_INCLUDED
# define VERTICALSLIDER_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

# include <vector>

class VerticalSlider: public UiElement {
    public:
        VerticalSlider (float* value, float min, float max, Vector2f const& topLeft, int height);

        void mouseLeft(bool down);
        void mouseMoved(Vector2f const& position);
        void keyEvent(bool down, Key const& key);

        void draw() const;

    private:
        float* value_;
        float minValue_, maxValue_;
};

# endif //VERTICALSLIDER_HPP_INCLUDED


