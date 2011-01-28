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

std::list<AmmoFist*> AmmoFist::activeParticles_;

AmmoFist::AmmoFist(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoFist>(spaceObjects::oAmmoFist, location, 20.f, 25.0f, 0.15f) {

    setDamageSource(damageSource);
    velocity_ = direction*500.f;
    location_ += velocity_*timer::frameTime()*1.2f;

}

void AmmoFist::update() {
    float time = timer::frameTime();

    physics::collide(this, MOBILES);

    location_ += velocity_*time;

    lifeTime_ += time;
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

