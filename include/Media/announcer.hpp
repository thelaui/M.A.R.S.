/* announcer.hpp

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

# ifndef ANNOUNCER_HPP_INCLUDED
# define ANNOUNCER_HPP_INCLUDED

/// A namespace for the commentator of the game.
/// The different moods which can be given to the
/// commentator will lead to different sayings.

namespace announcer {
    /// Several categories of sayings of the commentator.
    /// For each there are a lot of soundfiles which will
    /// be chosen randomly.
    enum SoundMood {Affronting, Praising, Neutral,
                    COUNT ///< COUNT contains the amount of entries in the enum. Keep it the last!
    };

    /// Updates the announcer.
    /// On slow motion it's pitch is changed.
    void update();

    /// Plays a soundfile based on the given mood.
    /// For each mood there are a lot of soundfiles
    /// which will be chosen randomly.
    void announce (SoundMood mood);
}

# endif // ANNOUNCER_HPP_INCLUDED
