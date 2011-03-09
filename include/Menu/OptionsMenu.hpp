/* OptionsMenu.hpp

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

# ifndef OPTIONSMENU_HPP_INCLUDED
# define OPTIONSMENU_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

class OptionsMenu: public UiWindow {
    public:
        static UiWindow* get();
        void checkWidgets();
        void onShow();

        static void reset();

    private:
        OptionsMenu(int width, int height): UiWindow(width, height) {}
        OptionsMenu(OptionsMenu const& copy);

        static UiWindow* instance_;

        static bool kOk_;

        static bool       fullscreen_, vsync_, shaders_;
        static sf::String resolution_, colorDepth_, format_;
        static int        soundVolume_, musicVolume_, announcerVolume_, starfield_;
};

# endif // OPTIONSMENU_HPP_INCLUDED



