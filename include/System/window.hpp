/* window.hpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

/// A namespace handling the main window of the game.

namespace window {
    /// Opens the window.
    /// Should be called once at the beginning.
    void open();

    /// Recreates the window.
    /// Can be called to apply settings::C_fullScreen.
    void create();

    /// Closes the window.
    /// Should be called once at the end.
    void close();

    /// Starts the main game loop.
    /// Should be called, when the window is opened a game is created.
    void mainLoop();

    /// Starts a draw cycle.
    /// The active OpenGL target is set to the window or the post effect backbuffer, depending
    /// on the use of shaders.
    void startDrawSpace();

    /// Finishs drawing the space and starts drawing the HUD.
    /// Applies post effects and sets the projection matrix to represent the screen in pixels.
    void startDrawHUD();

    /// Draws a SFML drawable in the window.
    void draw(sf::Drawable const& toBeDrawn);

    /// Returns a reference to the current input state of the window.
    /// This can be used for getting pressed keys and stuff like this.
    sf::Input const& getInput();

    /// Shows or hides the mouse cursor.
    void showCursor(bool show);

    /// Can be called to apply settings::C_vsync and settings::C_shaders.
    void applyGlobalSettings();

    /// Converts the given location from space coordinates (max (1280, 800)) to pixel coordinates on the screen.
    Vector2f const  coordToPixel(Vector2f const& spaceCoord);

    /// Returns a vector representing the size of the visible game area on screen.
    /// In pixels.
    Vector2f const& getViewPort();
}

# endif // WINDOW_HPP_INCLUDED
