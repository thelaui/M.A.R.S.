/* hud.hpp

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

# ifndef HUD_HPP_INCLUDED
# define HUD_HPP_INCLUDED

# define TEXT_ALIGN_LEFT   0
# define TEXT_ALIGN_CENTER 1
# define TEXT_ALIGN_RIGHT  2

# include "Media/font.hpp"
# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

namespace hud {
    void update();
    void draw();

    void drawSpaceText(std::string const& text, Vector2f const& location, font::FontType type = font::HandelGotDLig,
                       float size = 12.f, int align = TEXT_ALIGN_CENTER, Color3f const& color = Color3f(1.0, 1.0, 1.0),
                       Vector2f const& velocity = Vector2f());

    void drawScreenText(std::string const& text, Vector2f const& location, font::FontType type = font::HandelGotDLig,
                       float size = 12.f, int align = TEXT_ALIGN_LEFT, Color3f const& color = Color3f(1.0, 1.0, 1.0));

    float getCharacterPos(std::string const& text, int pos, font::FontType type = font::HandelGotDLig, float size = 12.f,
                          int align = TEXT_ALIGN_LEFT);

    void displayPoints();
    void displayStats(bool show = true);
    void refreshTabStats();
}

# endif // HUD_HPP_INCLUDED


