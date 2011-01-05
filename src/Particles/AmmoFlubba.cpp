/* AmmoFlubba.cpp

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

# include "Particles/AmmoFlubba.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "Media/sound.hpp"

std::list<AmmoFlubba*> AmmoFlubba::activeParticles_;

AmmoFlubba::AmmoFlubba(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoFlubba>(spaceObjects::oAmmoFlubba, location, 8.f, 0.4f, sf::Randomizer::Random(12.f, 15.f)) {

    setDamageSource(damageSource);
    velocity_ = velocity + direction*700;
    location_ += velocity_*timer::frameTime()*1.2f;

    radius_ = sf::Randomizer::Random(6.f, 8.f);

    color_ = Color3f(sf::Randomizer::Random(0.0f, 0.4f), sf::Randomizer::Random(0.8f, 1.f), sf::Randomizer::Random(0.0f, 0.4f));
}

void AmmoFlubba::update() {
    float time = timer::frameTime();

    physics::collide(this, STATICS | MOBILES | PARTICLES);
    Vector2f acceleration = physics::attract(this)*0.8f;

    // update Size
    if (lifeTime_ > totalLifeTime_-0.3f)
        radius_ = -400.0*pow(lifeTime_+0.2-totalLifeTime_, 2)+12;

    location_ += velocity_*time + acceleration*time*time;
    velocity_ += acceleration*time - 2.5f*velocity_*time;

    lifeTime_ += time;

    if (lifeTime_ > totalLifeTime_) {
        particles::spawnMultiple(2, particles::pMud, location_, Vector2f(), Vector2f(), color_);
        int rand = sf::Randomizer::Random(8, 20);
        sound::playSound(sound::BlubCollide, location_);
        for (int i=0; i<rand; ++i)
            particles::spawn(particles::pMiniAmmoFlubba, location_, Vector2f(), Vector2f(), Color3f(), damageSource_);
    }
}

void AmmoFlubba::draw() const {
    color_.gl4f(0.8f);
    const int posX = 4;
    const int posY = 0;
    glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
    glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
    glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
}

void AmmoFlubba::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    if (!isDead()) {
        physics::causeShockWave(damageSource(), location_, 350.f, 100.f, 0.f);
        sound::playSound(sound::BlubCollide, location_);
        killMe();
    }
}

void AmmoFlubba::shockWave(Vector2f const& location, float strength, float radius) {
    for (std::list<AmmoFlubba*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it) {
        Vector2f direction((*it)->location_ - location);
        float distance = direction.length();
        if (distance < radius && direction != Vector2f()) {
            float intensity = radius-distance;
            direction = direction.normalize();
            direction *= intensity;
            (*it)->velocity_ += direction;
        }
    }
}
