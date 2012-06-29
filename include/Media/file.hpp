/* file.hpp

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

# ifndef FILE_HPP_INCLUDED
# define FILE_HPP_INCLUDED

# include <SFML/System.hpp>
# include <string>
# include <vector>

/// A namespace for UTF-8 encoded text file handling.
/// It contains function which work with
/// UTF-8 encoded text files.

namespace file {
    /// Loads an UTF-8 encoded text file.
    /// The lines of this file are stored in a std::vector of SFML-Strings.
    /// Comments (//) and empty lines in this file will be ignored.
    bool load(std::string fileName, std::vector<sf::String>& strings);
}

# endif // FILE_HPP_INCLUDED


