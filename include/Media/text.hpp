/* text.hpp

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

# ifndef TEXT_HPP_INCLUDED
# define TEXT_HPP_INCLUDED

# define TEXT_ALIGN_LEFT   0
# define TEXT_ALIGN_CENTER 1
# define TEXT_ALIGN_RIGHT  2

# include "Media/font.hpp"
# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

namespace text {
    void drawSpaceText      (std::string const&, Vector2f const&, font::FontType, float size, int align, Color3f const&);
    void drawMobileSpaceText(std::string const&, Vector2f const&, font::FontType, float size, int align, Color3f const&);

    void drawScreenText     (std::string const&, Vector2f const&, font::FontType, float size, int align, Color3f const&);

    void drawFooText();

    float getCharacterPos   (std::string const&, int pos, font::FontType, float size, int align);
}

# endif // TEXT_HPP_INCLUDED



