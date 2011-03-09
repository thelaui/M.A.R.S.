/* NoSpecial.cpp

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

# include "Specials/NoSpecial.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "Games/games.hpp"
# include "Teams/Team.hpp"

# include <SFML/Graphics.hpp>

void NoSpecial::draw(float alpha) const {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // draw glow
    parent_->getOwner()->team()->color().brightened().gl4f(0.7f*alpha);

    const int posX = 3;
    const int posY = 3;

    glBegin(GL_QUADS);
        glTexCoord2f( posX*0.25f,    posY*0.25f);    glVertex2f(-parent_->radius()*4,-parent_->radius()*4);
        glTexCoord2f( posX*0.25f,   (posY+1)*0.25f); glVertex2f(-parent_->radius()*4, parent_->radius()*4);
        glTexCoord2f((posX+1)*0.25f,(posY+1)*0.25f); glVertex2f( parent_->radius()*4, parent_->radius()*4);
        glTexCoord2f((posX+1)*0.25f, posY*0.25f);    glVertex2f( parent_->radius()*4,-parent_->radius()*4);
    glEnd();
}
