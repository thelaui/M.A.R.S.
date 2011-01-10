/* BurningFragment.cpp

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

# include "Particles/BurningFragment.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "TrailEffects/trailEffects.hpp"

std::list<BurningFragment*> BurningFragment::activeParticles_;

BurningFragment::BurningFragment(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<BurningFragment>(spaceObjects::oBurningFragment, location, 1.f, 0, sf::Randomizer::Random(4.5f, 5.5f)),
         color_(sf::Randomizer::Random(0.7f, 1.f), sf::Randomizer::Random(0.7f, 1.f), 0.f),
         timer1_(0.5f),
         timer2_(0.25f) {

    radius_   = sf::Randomizer::Random(0.5f, 5.0f);
    velocity_ = Vector2f::randDir()*sf::Randomizer::Random(200, 600);

    trailEffects::attach(this, 10, 0.3f, radius_*2.f, Color3f(0.5f, 0.f, 0.f));
    trailEffects::attach(this, 5, 0.1f, radius_, color_);
}

BurningFragment::~BurningFragment() {
    trailEffects::detach(this);
}

void BurningFragment::update() {
    float time = timer::frameTime();
    Vector2f acceleration = physics::attract(this);
    physics::collide(this, STATICS | MOBILES);

    location_ += velocity_*time + acceleration*time*time*2;
    velocity_ += acceleration*time*2 + velocity_*-time;

    if (timer1_ > 0)
        timer1_ -= time;
    else {
        timer1_ = lifeTime_*4.f/settings::C_globalParticleCount;
        particles::spawn(particles::pSmoke, location_, velocity_);
    }
    if (timer2_ > 0)
        timer2_ -= time;
    else {
        timer2_ = lifeTime_/settings::C_globalParticleCount;
        particles::spawn(particles::pFragmentFlame, location_, Vector2f(), velocity_);
    }

    lifeTime_ += time;
}

void BurningFragment::draw() const {
    color_.gl4f(-1.0/totalLifeTime_*lifeTime_+1);
    const int posX = 5;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void BurningFragment::onCollision(SpaceObject* with, Vector2f const& location,
                                  Vector2f const& direction, Vector2f const& velocity) {

    if ((with->type() == spaceObjects::oHome | with->type() == spaceObjects::oSun) | (with->type() == spaceObjects::oPlanet)) {
        particles::spawn(particles::pMiniFlame, location_);
        particles::spawn(particles::pMiniFlame, location_);
        particles::spawn(particles::pMiniFlame, location_);
        particles::spawnMultiple(2, particles::pMud, location, direction, velocity, Color3f(1.f, 0.8f, 0.2f));
    }
    killMe();
}

