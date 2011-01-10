/* postFX.hpp

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

# ifndef POSTFX_HPP_INCLUDED
# define POSTFX_HPP_INCLUDED

# include "System/Vector2f.hpp"

# include <SFML/Graphics.hpp>

/// A namespace for post processing shaders.

namespace postFX {
    /// Updates the internal bumpmap GLSL shader.
    /// It will not do so, when settings::C_shaders is set to false.
    void update();

    /// Notifies the shader of explosions.
    /// For flash effects.
    void onExplosion();

    /// Returns a pointer to the active shader.
    /// If an error occured, it will return NULL.
    sf::Shader* get();

    /// Checks, whether GLSL shaders are supported on the current hardware.
    bool supported();

    /// Loads and compiles the post processing shader.
    /// Has to be called once at the beginnig of the program.
    void load();
}

# endif // POSTFX_HPP_INCLUDED


