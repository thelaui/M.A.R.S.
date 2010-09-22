/* UiElement.hpp

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

# include <SFML/Window.hpp>
# include <string>

class UiElement {
    public:
        UiElement(Vector2f const& topLeft, int width, int height);

        virtual void mouseMoved(Vector2f const& position);
        virtual void mouseLeft(bool down);
        virtual void buttonPressed(sf::Key::Code keyCode) {}
        virtual void textEntered(int keyCode) {}

        virtual void draw () const = 0;

        void setParent(UiElement* newParent);

        bool isHoovered() const              {return hoovered_;}
        bool isPressed()  const              {return pressed_; }
        virtual Vector2f getTopLeft();

    protected:
        UiElement* parent_;
        mutable Vector2f topLeft_;
        int width_, height_;

        bool hoovered_;
        bool pressed_;
};

# endif
