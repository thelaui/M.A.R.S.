/* UiWindow.hpp

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

# ifndef UIWINDOW_HPP_INCLUDED
# define UIWINDOW_HPP_INCLUDED

# include "Interface/UiElement.hpp"

# include <vector>

class UiWindow: public UiElement {
    public:
        UiWindow (int width, int height, Vector2f const& position = Vector2f());
        ~UiWindow();

        void mouseMoved(Vector2f const& position);
        void mouseWheelMoved(Vector2f const& position, int delta);
        void mouseLeft(bool down);
        void keyEvent(bool down, Key const& key);
        void textEntered(int keyCode);

        bool tabNext();
        bool tabPrevious();

        virtual void draw () const;

        void setFocus  (UiElement* toBeFocused, bool isPrevious);
        void clearFocus();

        virtual void checkWidgets() = 0;
        virtual void onShow() = 0;

        void addWidget (UiElement* toBeAdded);

        void setTopMost(bool);
        bool isTopMost() const {return topMost_;}
        bool isTabable() const {return false;}

        virtual Vector2f getTopLeft() const;

    protected:
        std::vector<UiElement*> widgets_;
        UiElement* focusedWidget_;

    private:
        bool topMost_;
};

# endif
