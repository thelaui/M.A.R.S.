/* Slider.hpp

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

# ifndef SLIDER_HPP_INCLUDED
# define SLIDER_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

# include <vector>

class Slider: public UiElement {
    public:
        Slider (sf::String* text, sf::String* toolTip, int* value, int min, int max, Vector2f const& topLeft, int width, int labelWidth=185, bool showValue=false, std::vector<sf::String> const& sliderNames = std::vector<sf::String>() );
        ~Slider ();

        void mouseLeft(bool down);
        void mouseMoved(Vector2f const& position);
        void keyEvent(bool down, sf::Key::Code keyCode);

        void draw() const;

        void setFocus  (UiElement* toBeFocused, bool isPrevious);
        void clearFocus();

    private:
        int* value_;
        int minValue_, maxValue_;
        Label* label_;
        int labelWidth_;
        bool showValue_;
        std::vector<sf::String> sliderNames_;
        sf::String* toolTip_;
};

# endif

