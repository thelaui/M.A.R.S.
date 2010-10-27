/* Item.cpp

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

# include "Items/Item.hpp"

# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Ship.hpp"

void Item::update() {
    std::vector<Ship*> const& shipList = ships::getShips();
    for (std::vector<Ship*>::const_iterator it = shipList.begin(); it != shipList.end(); ++it)
        if ((*it)->getLife() > 0.f && ((*it)->location() - location_).lengthSquare() < std::pow(radius_ + (*it)->radius(),2)) {
            collected_ = true;
            ship_ = *it;
        }
    if (ship_) {
        if (!ship_->collectedItems_[type_])
            ship_->collectedItems_[type_] = this;
        else if (type_ != items::iCannonControl) {
            //items::removeItem(this);
            ship_->collectedItems_[type_] = this;
        }
    }
}

void Item::draw() const {
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
        glTexCoord2f(posX1*0.15625f,     (posY1+1)*0.15625f); glVertex2f(-radius_*2.f, +radius_*2.f);
        glTexCoord2f((posX1+1)*0.15625f, (posY1+1)*0.15625f); glVertex2f(+radius_*2.f, +radius_*2.f);
        glTexCoord2f((posX1+1)*0.15625f, posY1*0.15625f);     glVertex2f(+radius_*2.f, -radius_*2.f);
    glEnd();

    glLoadIdentity();
    glTranslatef(location_.x_, location_.y_, 0);
    float scale(std::sin(timer::totalTime() *7.f) / 4.f + 1.f);
    glScalef(scale, scale, 1.f);
    glColor3f(1.f, 1.f, 1.f);
    // item layer
    glBegin(GL_QUADS);
        const int posX2 = texX_;
        const int posY2 = texY_;
        glTexCoord2f(posX2*0.15625f,     posY2*0.15625f);     glVertex2f(-radius_, -radius_);
        glTexCoord2f(posX2*0.15625f,     (posY2+1)*0.15625f); glVertex2f(-radius_, +radius_);
        glTexCoord2f((posX2+1)*0.15625f, (posY2+1)*0.15625f); glVertex2f(+radius_, +radius_);
        glTexCoord2f((posX2+1)*0.15625f, posY2*0.15625f);     glVertex2f(+radius_, -radius_);
    glEnd();

    glPopMatrix();

}

Vector2f const& Item::location() const {
    return location_;
}

float Item::radius() const {
    return radius_;
}

Item::~Item() {
    if (ship_)
        ship_->collectedItems_[type_] = false;
}
