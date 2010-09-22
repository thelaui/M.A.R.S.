/* CannonControl.cpp

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

# include "Items/CannonControl.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"

CannonControl::CannonControl(Vector2f const& location):
    Item(location, 20.f),
    ship_(NULL),
    respawnLocation_(location) {}

Ship* CannonControl::update() {
    if (!collected_) {
        ship_ = Item::update();
        if (ship_ != NULL)
            ship_->collectedItems_[0] = true;
    }
    else {
        if (ship_->docked_) {
            collected_ = false;
            location_ = respawnLocation_;
            ship_->collectedItems_[0] = false;
            ship_ = NULL;
        }
        else if (ship_->getLife() == 0.f) {
            collected_ = false;
            location_ = ship_->location();
            ship_->collectedItems_[0] = false;
            ship_ = NULL;
        }
    }
    return NULL;
}

void CannonControl::draw() const {
    if (!collected_) {
        glEnable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPushMatrix();
        glTranslatef(location_.x_, location_.y_, 0);

        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::PoGlare));
        glColor4f(1.f, 0.3f, 0.5f, 1.f);
        glRotatef(fmod(timer::totalTime()*100.f, 360.f), 0.f, 0.f, 1.f);

        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex2f(-radius_, -radius_);
            glTexCoord2i(0, 1); glVertex2f(-radius_, radius_);
            glTexCoord2i(1, 1); glVertex2f(radius_, radius_);
            glTexCoord2i(1, 0); glVertex2f(radius_, -radius_);
        glEnd();

        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0);
        float scale(std::sin(timer::totalTime() *7.f) / 4.f + 1.f);
        glScalef(scale, scale, 1.f);
        glColor3f(1.f, 1.f, 1.f);

        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CannonControl));
        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex2f(-radius_, -radius_);
            glTexCoord2i(0, 1); glVertex2f(-radius_, radius_);
            glTexCoord2i(1, 1); glVertex2f(radius_, radius_);
            glTexCoord2i(1, 0); glVertex2f(radius_, -radius_);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
    }
    else {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(ship_->location_.x_, ship_->location_.y_, 0.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ship_->owner_->color().gl3f();

        glBegin(GL_LINE_STRIP);
            glVertex2f(0.f, 0.f);
            glVertex2f( - ship_->velocity_.x_ * 0.02f,  - ship_->velocity_.y_ * 0.002f - 25.f);
            glVertex2f( - ship_->velocity_.x_ * 0.024f,  - ship_->velocity_.y_ * 0.0026f - 33.f);
            glVertex2f( - ship_->velocity_.x_ * 0.04f,  - ship_->velocity_.y_ * 0.004f - 43.f);
        glEnd();

        if (fmod(timer::totalTime(), 0.5f) < 0.1) {
            glColor3f(1.f, 0.7f, 0.9f);
            glEnable(GL_TEXTURE_2D);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Glow));
            glBegin(GL_QUADS);
                glTexCoord2i(0, 0); glVertex2f( - ship_->velocity_.x_ * 0.042f -15,  - ship_->velocity_.y_ * 0.0042f - 43.f-15);
                glTexCoord2i(0, 1); glVertex2f( - ship_->velocity_.x_ * 0.042f -15,  - ship_->velocity_.y_ * 0.0042f - 43.f+15);
                glTexCoord2i(1, 1); glVertex2f( - ship_->velocity_.x_ * 0.042f +15,  - ship_->velocity_.y_ * 0.0042f - 43.f+15);
                glTexCoord2i(1, 0); glVertex2f( - ship_->velocity_.x_ * 0.042f +15,  - ship_->velocity_.y_ * 0.0042f - 43.f-15);
            glEnd();
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            glColor3f(1.f, 1.0f, 1.0f);
        }
        else {
            glColor3f(1.f, 0.7f, 0.9f);
        }
         glPointSize(8.f);
        glBegin(GL_POINTS);
            glVertex2f( - ship_->velocity_.x_ * 0.042f,  - ship_->velocity_.y_ * 0.0042f - 43.f);
        glEnd();

        glTranslatef( - ship_->velocity_.x_ * 0.042f,  - ship_->velocity_.y_ * 0.0042f - 43.f, 0.f);
        glRotatef(fmod(timer::totalTime()*-100.f, 360.f), 0.f, 0.f, 1.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::HasControl));
        glColor4f(0.8f, 0.2f, 0.4f, 0.2f);
        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex2f(-24, -24);
            glTexCoord2i(0, 1); glVertex2f(-24, 24);
            glTexCoord2i(1, 1); glVertex2f(24, 24);
            glTexCoord2i(1, 0); glVertex2f(24, -24);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);



        glPopMatrix();
    }
}

Player* CannonControl::getCarrier() const {
    if (ship_)
        return ship_->owner_;
    else
        return NULL;
}
