/* WeaponOptions.hpp

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

# ifndef WEAPONOPTIONS_HPP_INCLUDED
# define WEAPONOPTIONS_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

class WeaponOptions: public UiWindow {
    public:
        static UiWindow* get();
        void checkWidgets();
        void onShow();

        static void reset();

    private:
        WeaponOptions(int width, int height): UiWindow(width, height) {}
        WeaponOptions(WeaponOptions const& copy);

        static UiWindow* instance_;

        static bool kOk_;
        static bool kAFK_, kBurner_, kFist_, kFlubba, kShotgun_, kRocket_, kRofle_, kH2OMG_;
        static bool kFreeze_, kHeal_, kBlast_, kFireWall_;
};

# endif // WEAPONOPTIONS_HPP_INCLUDED







