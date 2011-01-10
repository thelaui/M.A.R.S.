/* Tab.hpp

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

# ifndef TAB_HPP_INCLUDED
# define TAB_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/Label.hpp"

# include <vector>

class Tab: public UiElement {
    public:
        Tab (sf::String* text, int width, bool* activated = NULL);
        ~Tab();

        void mouseMoved(Vector2f const& position);
        void mouseLeft(bool down);
        void keyEvent(bool down, sf::Key::Code keyCode);
        void textEntered(int keyCode);

        bool tabNext();
        bool tabPrevious();

        void draw () const;

        void setFocus  (UiElement* toBeFocused, bool isPrevious);
        void clearFocus();

        void addWidget (UiElement* toBeAdded);

        /* virtual */
        Vector2f getTopLeft();
        bool isActive() const {return active_;}

        friend class TabList;

    private:
        std::vector<UiElement*> widgets_;
        UiElement* focusedWidget_;
        sf::String* name_;
        Label* label_;
        bool* activated_;
        bool active_;
};

# endif



