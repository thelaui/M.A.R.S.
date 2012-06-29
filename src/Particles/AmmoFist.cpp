/* AmmoFist.cpp

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

# include "Particles/AmmoFist.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "Players/Player.hpp"

std::list<AmmoFist*> AmmoFist::activeParticles_;

AmmoFist::AmmoFist(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoFist>(spaceObjects::oAmmoFist, location, 20.f, 25.0f, 0.1f) {

    setDamageSource(damageSource);
}

void AmmoFist::update() {
    float time = timer::frameTime()*0.5f;

    for (int i=0; i<2; ++i) {
        float position = (lifeTime_)*17.f + 1.f;

        Vector2f dir(std::cos(damageSource_->ship()->rotation()*M_PI/180.f), std::sin(damageSource_->ship()->rotation()*M_PI/180.f));
        location_ = damageSource_->ship()->location();
        velocity_ = dir*500.f;
        radius_   = lifeTime_*250.f + 1.f;

        location_ += dir*position*damageSource_->ship()->radius()*(position+1);

        physics::collide(this, MOBILES | STATICS);

        lifeTime_ += time;
    }
}

void AmmoFist::draw() const {
    glColor3f(1, 1, 1);
    const int posX = 0;
    const int posY = 1;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void AmmoFist::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    float strength = (velocity-velocity_).length();

    if (strength > 50.f) {

        switch (with->type()) {
            case spaceObjects::oShip:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oBall:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oPlanet: case spaceObjects::oHome:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            case spaceObjects::oSun:
                sound::playSound(sound::Particle, location, (strength-50)/3);
                break;

            default:;
        }
    }

    killMe();
}

