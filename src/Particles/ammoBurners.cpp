/* ammoBurners.cpp

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

# include "Particles/ammoBurners.hpp"

# include "System/timer.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/balls.hpp"
# include "SpaceObjects/Ship.hpp"

namespace ammoBurners {

    namespace {
        std::list<AmmoBurner*> activeParticles_;
    }

    AmmoBurner::AmmoBurner(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Player* damageSource):
               Particle(spaceObjects::oAmmoBurner, location, 1.f, 0.f, sf::Randomizer::Random(0.4f, 0.5f)) {

        setDamageSource(damageSource);
        Vector2f distortion(Vector2f::randDirLen()*3.f);
        location_ = location + direction*sf::Randomizer::Random(0.f, 20.f);
        velocity_ = velocity + direction*700.f + distortion*70.f;

        color_.h(50.f);
        color_.v(0.8f);
        color_.s(0.3f);
    }

    void AmmoBurner::update() {
        float time = timer::frameTime();
        physics::collide(this, STATICS);
        // update Color
        color_.h((-1.0f/totalLifeTime_*lifeTime_+1.f)*60.f);
        color_.v(-0.8f/totalLifeTime_*lifeTime_+0.8f);
        color_.s(lifeTime_*10.f + 0.3f);

        // update Size
        radius_ = lifeTime_*50 + 2;

        location_ += velocity_*time;
        velocity_ += velocity_*(-4.f)*time;

        lifeTime_ += time;

        // check for collisions with ships
        std::vector<Ship*>const& shipsList = ships::getShips();
        for (std::vector<Ship*>::const_iterator it = shipsList.begin(); it != shipsList.end(); ++it)
            if ((location_ - (*it)->location()).lengthSquare() < std::pow(radius_ + (*it)->radius(), 2))
                (*it)->onCollision(this, location_, velocity_, velocity_);

        // check for collisions with ball
        Ball* ball = balls::getBall();
        if (ball)
            if ((location_ - ball->location()).lengthSquare() < std::pow(radius_ + ball->radius(), 2))
                ball->onCollision(this, location_, velocity_, velocity_);
    }

    void AmmoBurner::draw() const {
        color_.gl3f();
        const int posX = 0;
        const int posY = 0;
        glTexCoord2f(posX*0.125f,     posY*0.125f);     glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(posX*0.125f,     (posY+1)*0.125f); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, (posY+1)*0.125f); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f((posX+1)*0.125f, posY*0.125f);     glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }


    void spawn(Vector2f const& location, Vector2f const& direction, Vector2f const& sourceVelocity, Player* damageSource) {
        activeParticles_.push_back(new AmmoBurner(location, direction, sourceVelocity, damageSource));
    }

    void draw() {
        for (std::list<AmmoBurner*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            (*it)->draw();
    }

    void update() {
        std::list<AmmoBurner*>::iterator it = activeParticles_.begin();
        while (it != activeParticles_.end()) {
            (*it)->update();
            if ((*it)->isDead()) {
                delete *it;
                it = activeParticles_.erase(it);
            }
            else ++it;
        }
    }

    int count() {
        return activeParticles_.size();
    }

    void clear() {
        for (std::list<AmmoBurner*>::iterator it = activeParticles_.begin(); it != activeParticles_.end(); ++it)
            delete *it;
        activeParticles_.clear();
    }
}

