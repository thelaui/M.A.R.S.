/* ShipHighlight.cpp

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

# include "DecoObjects/ShipHighlight.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "Particles/AmmoInsta.hpp"

void ShipHighlight::draw() const {
    if (ship_->visible_) {
        // wobble when charging
        if ((ship_->docked_ && (ship_->getLife() < 100.f) | (ship_->getFuel() < 100.f)))
            draw(ship_->location(), std::sin(timer::totalTime()*10.f)*0.15f + 1.f);
        else
            draw(ship_->location(), 1.f);
    }
    else if (ship_->respawnTimer_ < 0.5f) {
        draw(ship_->respawnLocation_, 1.f + ship_->respawnTimer_*10.f);
    }
}

void ShipHighlight::draw(Vector2f const& location, float scale) const {
    const float    maxAngle     (ship_->currentWeapon_->maxAngle());
    const float    shipRotation (ship_->rotation_*M_PI/180.f);
    const Vector2f shipDirection(Vector2f(std::cos(shipRotation), std::sin(shipRotation)));
    if (ship_->currentWeapon_->getType() == weapons::wInsta) {
       // AmmoInsta::hitsAny(ship_->location() + shipDirection*ship_->radius(), shipDirection, ship_->owner_->team());
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ships));
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(location.x_, location.y_, 0.f);
    glRotatef(fmod(timer::totalTime()*100.f, 360.f), 0.f, 0.f, 1.f);

    ship_->owner_->color().brightened().gl4f(0.6f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.875f);       glVertex2f(-ship_->radius_*2.7f*scale,-ship_->radius_*2.7f*scale);
        glTexCoord2f(0.f, 1.f);          glVertex2f(-ship_->radius_*2.7f*scale, ship_->radius_*2.7f*scale);
        glTexCoord2f(0.125f,  1.f);      glVertex2f( ship_->radius_*2.7f*scale, ship_->radius_*2.7f*scale);
        glTexCoord2f(0.125f,  0.875f);   glVertex2f( ship_->radius_*2.7f*scale,-ship_->radius_*2.7f*scale);
    glEnd();

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

