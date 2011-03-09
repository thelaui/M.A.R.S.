/* music.hpp

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

# ifndef MUSIC_HPP_INCLUDED
# define MUSIC_HPP_INCLUDED

# include <SFML/Audio.hpp>

/// A namespace for playing music.
/// Music is streamed from OGG files.

namespace music {
    /// Updates the music player.
    /// If the ingame music has stopped, a new randomly
    /// chosen track will be played.
    void update();

    /// Plays a music file.
    void play(std::string fileName);
    /// Plays a random music file.
    void play();
    /// Switches to the next track.
    void next();
    /// Switches to the previous track.
    void previous();
    /// Stops playing Music.
    void stop();

    /// Silences the music slowly.
    void fadeOut();

    /// Applies the global music Volume.
    /// Which is set in \a settings::C_musicVolume. This is useful
    /// when this variable has been changed to apply the effect.
    void setGlobalVolume();
}

# endif // MUSIC_HPP_INCLUDED
