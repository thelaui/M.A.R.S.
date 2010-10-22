/* Timer.hpp

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

# ifndef TIMER_HPP_INCLUDED
# define TIMER_HPP_INCLUDED

# include <SFML/Graphics.hpp>

/// A namespace handling time.
/// The entire game is timed by this namespace.

namespace timer {
    /// Updates the timer.
    /// Has to be called every frame once.
    void update(float frameTime);

    /// Notifies the timer when a ship exploded, for slow motion managing.
    void onShipExplode();

    /// Returns the duration of the last frame in seconds.
    float frameTime();

    /// Returns the total duration of the game in seconds.
    float totalTime();

    /// Returns the current frames per second.
    /// Calculated twice a second.
    float fps();
}

# endif // TIMER_HPP_INCLUDED
