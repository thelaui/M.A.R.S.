/* TutWindow15.hpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

This program is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 7 of the License, or (at your option)
any later version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>. */

# ifndef TUTWINDOW15_HPP_INCLUDED
# define TUTWINDOW15_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

class TutWindow15: public UiWindow {
    public:
        static UiWindow* get();
        void checkWidgets();
        void onShow() {}

        static void reset();

    private:
        TutWindow15(int width, int height): UiWindow(width, height) {}
        TutWindow15(TutWindow15 const& copy);

        static UiWindow* instance_;

        static bool kOk_;
};

# endif // TUTWINDOW15_HPP_INCLUDED







