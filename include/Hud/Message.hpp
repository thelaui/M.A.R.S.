/* Message.hpp

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

# ifndef MESSAGE_HPP_INCLUDED
# define MESSAGE_HPP_INCLUDED

# include "Hud/HudElement.hpp"
# include "System/Color3f.hpp"
# include "System/window.hpp"

class Message: public HudElement {
    public:
        Message(sf::String const& message, Color3f const& color);

        void draw() const;
        void update();

        bool isDead() const;

    private:
        sf::String text_;
        Color3f    color_;
        Vector2f   location_;
        float      timer_;
        float      alpha_;
        float      speed_;
};

# endif // MESSAGE_HPP_INCLUDED







