/* Cannon.cpp

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

# include "DecoObjects/Cannon.hpp"

# include "Media/texture.hpp"
# include "System/timer.hpp"
# include "Players/Player.hpp"
# include "Items/items.hpp"
# include "Items/CannonControl.hpp"
# include "Teams/Team.hpp"
# include "Particles/particles.hpp"
# include "SpaceObjects/Home.hpp"
# include "SpaceObjects/ships.hpp"
# include "Teams/teams.hpp"
# include "defines.hpp"

Cannon::Cannon():
    timer_(0.f),
    rotation_(0.f) {}

void Cannon::update() {
    Player* carrier = items::getCannonControl()->getCarrier();

    Vector2f toTarget;
    if (carrier && teams::getEnemy(carrier->team())->home()->getLife() > 0 && carrier->team()->home()->getLife() > 0)
        toTarget = (teams::getEnemy(carrier->team())->home()->location() - Vector2f(SPACE_X_RESOLUTION*0.5f, 0.f)).normalize();
    else
        toTarget = Vector2f(0.f, -1.f);

    float angle = std::acos(toTarget*Vector2f(1.f, 0.f))-M_PI/2;

    if (std::abs(angle-rotation_*M_PI/180) > 0.01f) {
        timer_ = 0.f;
        const float rotSpeed(timer::frameTime()*4*ships::getShips().size());
        if (angle > 0.f) {
            if (angle > rotation_*M_PI/180)
                rotation_ += rotSpeed;
            else
                rotation_ -= rotSpeed;
        }
        else {
            if (angle < rotation_*M_PI/180)
                rotation_ -= rotSpeed;
            else
                rotation_ += rotSpeed;
        }
    }
    else if (std::abs(angle) > 0.1f) {
        timer_ += timer::frameTime();
        Vector2f direction(-std::sin(rotation_*M_PI/180), std::cos(rotation_*M_PI/180));
        Vector2f location(Vector2f(SPACE_X_RESOLUTION*0.5f, 0.f) + direction*180.f);
        const float shootSpeed(15.f/ships::getShips().size());
        if (timer_ > shootSpeed) {
            timer_ = 0.f;
            particles::spawn(particles::pCannonBall, location, direction);
            ++carrier->cannonShots_;
            ++carrier->points_;
        }
        if (timer_ > shootSpeed*0.5f)
            particles::spawnTimed(std::pow((timer_*2.f/shootSpeed)-1, 3), particles::pDust, location);
    }
}

void Cannon::draw() const {
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::CannonSocket));
    glColor3f(1,1,1);
    glTranslatef(SPACE_X_RESOLUTION*0.5f, 0.f, 0.f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f); glVertex2f(-100.f, 0.f);
        glTexCoord2f(0.f, 0.5f); glVertex2f(-100.f, 100.f);
        glTexCoord2f(1.f, 0.5f); glVertex2f( 100.f, 100.f);
        glTexCoord2f(1.f, 0.f); glVertex2f( 100.f, 0.f);
    glEnd();

    glRotatef(rotation_, 0.f, 0.f, 1.f);

    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Cannon));

    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(-100.f, 0.f);
        glTexCoord2i(0, 1); glVertex2f(-100.f, 200.f);
        glTexCoord2i(1, 1); glVertex2f( 100.f, 200.f);
        glTexCoord2i(1, 0); glVertex2f( 100.f, 0.f);
    glEnd();


    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
}
