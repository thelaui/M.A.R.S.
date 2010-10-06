/* BlackHole.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "SpaceObjects/BlackHole.hpp"

# include "System/timer.hpp"

# include <cmath>

BlackHole::BlackHole(Vector2f const& location, float radius):
                     SpaceObject(spaceObjects::oBlackHole, location, radius, radius*130),
                     rotation_(0) {
    physics::addGravitySource(this);
}

void BlackHole::update() {
    rotation_ = fmod(rotation_ += 10*timer::frameTime(), 360);
}

void BlackHole::draw() const {
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(location_.x_, location_.y_, 0);
    glRotatef(rotation_, 0, 0, 1);

    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Hole1));

    glColor4f(1,1,1,0.2);
    float drawRadius = radius_*1.8f;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(-drawRadius, -drawRadius);
        glTexCoord2i(0, 1); glVertex2f(-drawRadius,  drawRadius);
        glTexCoord2i(1, 1); glVertex2f( drawRadius,  drawRadius);
        glTexCoord2i(1, 0); glVertex2f( drawRadius, -drawRadius);
    glEnd();
    glColor4f(1,1,1,0.6);
    glRotatef(rotation_*-2, 0, 0, 1);
    drawRadius = radius_*1.3f;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(-drawRadius, -drawRadius);
        glTexCoord2i(0, 1); glVertex2f(-drawRadius,  drawRadius);
        glTexCoord2i(1, 1); glVertex2f( drawRadius,  drawRadius);
        glTexCoord2i(1, 0); glVertex2f( drawRadius, -drawRadius);
    glEnd();
    glColor4f(1,1,1,1);
    glRotatef(rotation_*4, 0, 0, 1);
    drawRadius = radius_;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(-drawRadius, -drawRadius);
        glTexCoord2i(0, 1); glVertex2f(-drawRadius,  drawRadius);
        glTexCoord2i(1, 1); glVertex2f( drawRadius,  drawRadius);
        glTexCoord2i(1, 0); glVertex2f( drawRadius, -drawRadius);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}


