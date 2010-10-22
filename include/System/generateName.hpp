/* generateName.hpp

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

# ifndef GENERATENAME_HPP_INCLUDED
# define GENERATENAME_HPP_INCLUDED

# include <vector>
# include <SFML/System.hpp>
# include <SFML/Window.hpp>

/// A namespace for genearing names.
/// It generates names for bots, the game itself and
/// stores the names for the ships.

namespace generateName {
    /// Returns a random bot name.
    /// The name is loaded from the file "botnames.txt".
    /// \param randomNumber chooses the group of names from which the returned
    ///        name will be chosen. For bots in the same team, use the same number.
    sf::String bot(int randomNumber);

    /// Returns a random name for the game.
    /// For instance "Retarded Shooter" or "Rude Shooter".
    sf::String game();

    /// Returns a reference to a vector containing the ship names.
    /// they are loaded from the file "shipnames.txt"
    std::vector<sf::String> const& shipNames();

    /// Returns a human-readable string, containing the name of given key.
    /// The key is loaded from the current locale.
    sf::String const key(sf::Key::Code keyCode);
}

# endif //GENERATENAME_HPP_INCLUDED
