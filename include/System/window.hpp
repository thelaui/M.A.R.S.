/* window.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# ifndef WINDOW_HPP_INCLUDED
# define WINDOW_HPP_INCLUDED

# include "System/Vector2f.hpp"

# include <SFML/Graphics.hpp>

namespace window {
    sf::RenderWindow* getMainWindow();

    bool isOpen();
    void handleEvents();
    void resized();
    void setPixelView();
    void setSpaceView();
    void display();
    void clear();

    void showCursor(bool show);
    void create();

    Vector2f coordToPixel(Vector2f const& spaceCoord);
    Vector2f pixelToCoord(Vector2f const& pixelCoord);
    Vector2f const& getViewPort();

    void close();
}

# endif // WINDOW_HPP_INCLUDED
