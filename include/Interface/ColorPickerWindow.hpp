/* ColorPickerWindow.hpp

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

# ifndef COLORPICKERWINDOW_HPP_INCLUDED
# define COLORPICKERWINDOW_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

class Color3f;
class ColorPicker;
class Button;

class ColorPickerWindow: public UiWindow {
    public:
        ColorPickerWindow (ColorPicker* parent, Color3f* color);

        void draw() const;

        void checkWidgets();
        void onShow() {};

        void reset() {};

    private:
        Button* Ok_;
        bool kOk_;
        ColorPicker* parent_;
        Color3f*  color_;
};

# endif // COLORPICKERWINDOW_HPP_INCLUDED
