/* UiElement.hpp

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

# ifndef UIELEMENT_HPP_INCLUDED
# define UIELEMENT_HPP_INCLUDED

# include "System/Vector2f.hpp"

# include "System/Key.hpp"

# include <SFML/Window.hpp>

class UiElement {
    public:
        UiElement(Vector2f const& topLeft, int width, int height);

        virtual void mouseMoved(Vector2f const& position);
        virtual void mouseWheelMoved(Vector2f const& position, int delta) {}
        virtual void mouseLeft(bool down);
        virtual void keyEvent(bool down, Key const& key) {}
        virtual void textEntered(sf::Uint32 keyCode) {}
        virtual bool tabNext() {return true;}
        virtual bool tabPrevious() {return true;}

        virtual void draw() const;

        void setParent(UiElement* newParent);
        virtual void setFocus  (UiElement* toBeFocused, bool isPrevious);
        virtual void clearFocus();

        bool         isHovered() const {return hovered_;}
        bool         isPressed() const {return pressed_;}
        bool         isFocused() const {return focused_;}
        virtual bool isTopMost() const {return parent_->isTopMost();}
        virtual bool isTabable() const {return true;}
        virtual Vector2f getTopLeft() const;

    protected:
        int width() const;

        UiElement* parent_;
        mutable Vector2f topLeft_;
        int width_, height_;

        bool hovered_;
        bool focused_;
        bool pressed_;

        mutable float hoveredFadeTime_;
        mutable float focusedFadeTime_;
};

# endif
