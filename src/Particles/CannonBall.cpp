/* CannonBall.cpp

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

# include "Particles/CannonBall.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "SpaceObjects/Home.hpp"
# include "SpaceObjects/ships.hpp"

std::list<CannonBall*> CannonBall::activeParticles_;

CannonBall::CannonBall(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<CannonBall>(spaceObjects::oCannonBall, location, 25.f, 30.0f, 5.0f),
         timer1_(0.f),
         timer2_(0.f) {

    velocity_ = direction*300.f;
}

void CannonBall::update() {
    float time = timer::frameTime();
    location_ += velocity_*time;

    if (timer1_ > 0)
        timer1_ -= time;
    else {
        timer1_ = 0.03f/settings::C_globalParticleCount;
        for (int i=0; i<20; ++i) {
            particles::spawn(particles::pSmoke, location_+Vector2f::randDirLen()*15.f, velocity_);
            particles::spawn(particles::pDust, location_);
        }
    }

    if (timer2_ > 0)
        timer2_ -= time;
    else {
        timer2_ = 0.2f/settings::C_globalParticleCount;
        particles::spawn(particles::pFragmentFlame, location_, Vector2f(20.f,0), velocity_);
    }

    // check for collisions with homes
    std::vector<Home*>const& homes = spaceObjects::getHomes();
    for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it) {
        if ((location_ - (*it)->location()).lengthSquare() < std::pow(radius_ + (*it)->radius(), 2)) {
            (*it)->onCollision(this, Vector2f(), Vector2f(), Vector2f());
            killMe();
        }
    }

    // check for collisions with ships
    std::vector<Ship*>const& shipsList = ships::getShips();
    for (std::vector<Ship*>::const_iterator it = shipsList.begin(); it != shipsList.end(); ++it)
        if ((location_ - (*it)->location()).lengthSquare() < std::pow(radius_ + (*it)->radius(), 2) && (*it)->getLife() > 0.f) {
            (*it)->onCollision(this, Vector2f(), Vector2f(), Vector2f());
            killMe();
        }

    if (isDead()) {
        sound::playSound(sound::BallExplode, location_, 100.f);
        physics::causeShockWave(damageSource(), location_, 100.f, 300.f, 10.f);
        particles::spawnMultiple(5 , particles::pFragment, location_, location_, location_, Color3f(0.3f, 0.3f, 0.3f));
        particles::spawnMultiple(70, particles::pDust, location_);
        particles::spawnMultiple(20, particles::pExplode, location_);
        particles::spawnMultiple(8, particles::pBurningFragment, location_);
        particles::spawnMultiple(1, particles::pMiniFlame, location_);
    }
}

void CannonBall::draw() const {
    glColor3f(1.0f, 1.0f, 1.0f);
    const int posX = 5;
    const int posY = 2;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+3)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+3)*0.125f, (posY+3)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+3)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

