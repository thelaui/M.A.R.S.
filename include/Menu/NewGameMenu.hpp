/* NewGameMenu.hpp

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

# ifndef NEWGAMEMENU_HPP_INCLUDED
# define NEWGAMEMENU_HPP_INCLUDED

# include "Interface/UiWindow.hpp"

class NewGameMenu: public UiWindow {
    public:
        static UiWindow* get();
        void checkWidgets();
        void onShow();

    private:
        NewGameMenu(int width, int height): UiWindow(width, height) {}
        NewGameMenu(NewGameMenu const& copy);

        static UiWindow* instance_;

        static bool kStartSB_, kStartDM_, kStartTDM_, kStartCK_, kInfoSB_,
                    kInfoDM_,  kInfoTDM_, kInfoCK_,   kCancel_, playerI_, playerII_;
};

# endif // NEWGAMEMENU_HPP_INCLUDED


