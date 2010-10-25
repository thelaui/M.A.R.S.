/* PUShield.cpp

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

# include "Items/PUShield.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"

void PUShield::draw() const {
    if (!collected()) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPushMatrix();
        glTranslatef(location_.x_, location_.y_, 0);

        glColor4f(1.f, 0.6f, 0.8f, 1.f);
        glRotatef(fmod(timer::totalTime()*100.f, 360.f), 0.f, 0.f, 1.f);
        // glow
        glBegin(GL_QUADS);
            const int posX1 = 0;
            const int posY1 = 0;
            glTexCoord2f(posX1*0.15625f,     posY1*0.15625f);     glVertex2f(-radius_*2.f, -radius_*2.f);
            glTexCoord2f(posX1*0.15625f,     (posY1+1)*0.15625f); glVertex2f(-radius_*2.f, radius_*2.f);
            glTexCoord2f((posX1+1)*0.15625f, (posY1+1)*0.15625f); glVertex2f(radius_*2.f, radius_*2.f);
            glTexCoord2f((posX1+1)*0.15625f, posY1*0.15625f);     glVertex2f(radius_*2.f, -radius_*2.f);
        glEnd();

        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0);
        float scale(std::sin(timer::totalTime() *7.f) / 4.f + 1.f);
        glScalef(scale, scale, 1.f);
        glColor3f(1.f, 1.f, 1.f);
        // shield
        glBegin(GL_QUADS);
            const int posX2 = 4;
            const int posY2 = 0;
            glTexCoord2f(posX2*0.15625f,     posY2*0.15625f);     glVertex2f(-radius_, -radius_);
            glTexCoord2f(posX2*0.15625f,     (posY2+1)*0.15625f); glVertex2f(-radius_, radius_);
            glTexCoord2f((posX2+1)*0.15625f, (posY2+1)*0.15625f); glVertex2f(radius_, radius_);
            glTexCoord2f((posX2+1)*0.15625f, posY2*0.15625f);     glVertex2f(radius_, -radius_);
        glEnd();

        glPopMatrix();
    }
    else {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ship()->location().x_, ship()->location().y_, 0.f);

        if (fmod(timer::totalTime(), 0.5f) < 0.1) {
            glColor3f(1.f, 0.7f, 0.9f);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            // flash
            glBegin(GL_QUADS);
                const int posX3 = 0;
                const int posY3 = 0;
                glTexCoord2f(posX3*0.15625f,     posY3*0.15625f);     glVertex2f( - ship()->velocity().x_ * 0.042f -15,  - ship()->velocity().y_ * 0.0042f - 43.f-15);
                glTexCoord2f(posX3*0.15625f,     (posY3+1)*0.15625f); glVertex2f( - ship()->velocity().x_ * 0.042f -15,  - ship()->velocity().y_ * 0.0042f - 43.f+15);
                glTexCoord2f((posX3+1)*0.15625f, (posY3+1)*0.15625f); glVertex2f( - ship()->velocity().x_ * 0.042f +15,  - ship()->velocity().y_ * 0.0042f - 43.f+15);
                glTexCoord2f((posX3+1)*0.15625f, posY3*0.15625f);     glVertex2f( - ship()->velocity().x_ * 0.042f +15,  - ship()->velocity().y_ * 0.0042f - 43.f-15);
            glEnd();

            glColor3f(1.f, 1.0f, 1.0f);
        }
        else {
            glColor3f(1.f, 0.7f, 0.9f);
        }

        // shield bubble
        glColor4f(1.0f, 0.5f, 0.8f, 0.9f);
        glBegin(GL_QUADS);
                const int posX = 5;
                const int posY = 0;
                glTexCoord2f(posX*0.15625f,     posY*0.15625f);     glVertex2f(-35, -35);
                glTexCoord2f(posX*0.15625f,     (posY+1)*0.15625f); glVertex2f(-35, +35);
                glTexCoord2f((posX+1)*0.15625f, (posY+1)*0.15625f); glVertex2f(+35, +35);
                glTexCoord2f((posX+1)*0.15625f, posY*0.15625f);     glVertex2f(+35, -35);
        glEnd();

        glPopMatrix();
    }
}
