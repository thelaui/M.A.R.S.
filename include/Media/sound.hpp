/* sound.hpp

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

# ifndef SOUND_HPP_INCLUDED
# define SOUND_HPP_INCLUDED

# include "System/Vector2f.hpp"

# include <SFML/Audio.hpp>

/// A namespace for playing sound effects.
/// It is possible to choose which sound to play,
/// it's volume and where to play it (for stereo effects).

namespace sound {
    /// All different sound effects.

    enum SoundType {Pump, Laser, BallExplode, ShipExplode, ShipCollide,  BallPlanetCollide,
                    Particle, ShipBallCollide, BallRespawn, PlanetExplode, PumpShipCollide,
                    ShipPlanetCollide, ShipRespawn, BlubPop, Fuel, LaserCollide, NoFuel,
                    Sniper, Blub, BlubCollide, SniperShipCollide, SniperPlanetCollide, Click,
                    Tab, Check, Countdown, Start, Swish, Score,
                    COUNT ///< COUNT contains the amount of entries in the enum. Keep it the last!
    };

    /// Plays a sound file.
    /// \param type specifies the sound::SoundType
    /// \param position selects the location on the screen, where the sound should be played
    /// \param sets the volume
    void playSound (SoundType type, Vector2f const& position = Vector2f(640, 0), float volume = 100.f);
}

# endif // SOUND_HPP_INCLUDED
