/* AmmoInsta.cpp

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

# include "Particles/AmmoInsta.hpp"

# include "System/timer.hpp"
# include "Media/sound.hpp"
# include "TrailEffects/trailEffects.hpp"
# include "TrailEffects/Trail.hpp"
# include "System/Vector2f.hpp"
# include "Teams/Team.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "System/settings.hpp"

std::list<AmmoInsta*> AmmoInsta::activeParticles_;

AmmoInsta::AmmoInsta(Vector2f const& location, Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource):
         Particle<AmmoInsta>(spaceObjects::oAmmoInsta, location, 1.f, 3.0f, 10.0f),
         color_(color) {

    setDamageSource(damageSource);
    velocity_ = direction.normalize()*1000.f;
    location_ += velocity_*timer::frameTime()*1.2f;

    trail_ = trailEffects::attach(this, 0.1f, 2.f, 6.f, color_, true);

    color_.v(1.f);
    color_.s(0.3f);
}

AmmoInsta::~AmmoInsta() {
    trailEffects::detach(this);
}


void AmmoInsta::update() {
    const int steps(5);
    float time = timer::frameTime()/steps;

    for (int i=0; i<steps; ++i) {
        if (!isDead()) {
            physics::collide(this, STATICS | MOBILES);
            Vector2f acceleration = physics::attract(this)*70;

            location_ += velocity_*time + acceleration*time*time;
            velocity_ += acceleration*time;

            lifeTime_ += time;

            if (i!=steps-1)
                trail_->update();
        }
        else break;
    }

    if (location_.x_ < -100 || location_.x_ > 1280 + 100 || location_.y_ < -100 || location_.y_ > 800 + 100) {
        killMe();
    }
}

void AmmoInsta::draw() const {
    color_.gl3f();

    Vector2f direction(velocity_*0.015f);
    Vector2f normDirection(direction.y_, -1.f*direction.x_);
    normDirection *= 0.13f;

    const Vector2f topLeft(location_ + 2*direction + normDirection), topRight(location_ + 2*direction - normDirection), bottomLeft(location_ - 2*direction + normDirection), bottomRight(location_ - 2*direction - normDirection);

    const int posX = 0;
    const float posY = 7.f;
    glTexCoord2f(posX*0.125f,       posY*0.125f);       glVertex2f(topLeft.x_, topLeft.y_);
    glTexCoord2f(posX*0.125f,      (posY+0.5f)*0.125f); glVertex2f(bottomLeft.x_, bottomLeft.y_);
    glTexCoord2f((posX+3.f)*0.125f,(posY+0.5f)*0.125f); glVertex2f(bottomRight.x_, bottomRight.y_);
    glTexCoord2f((posX+3.f)*0.125f, posY*0.125f);       glVertex2f(topRight.x_, topRight.y_);
}

void AmmoInsta::onCollision(SpaceObject* with, Vector2f const& location,
                        Vector2f const& direction, Vector2f const& velocity) {
    float strength = (velocity-velocity_).length();

    if (strength > 50.f) {

        switch (with->type()) {
            case spaceObjects::oShip:
                sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oBall:
                sound::playSound(sound::SniperShipCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oPlanet: case spaceObjects::oHome:
                sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                break;

            case spaceObjects::oSun:
                sound::playSound(sound::SniperPlanetCollide, location, (strength-50)/3);
                break;

            default:;
        }
    }
    killMe();
}

bool AmmoInsta::hitsAny(Vector2f const& location, Vector2f const& direction, Team* team) {

    //glLineWidth(2.f);
    //team->color().gl4f(0.4f);

    float resolution(0.05f + 0.002f*(100-settings::C_iDumb));

    Vector2f velocity(direction.normalize()*1000.f), from(location);
    from += velocity*timer::frameTime()*1.2f;

    for (int i=0; i<10+settings::C_iDumb*0.4f; ++i) {
        Vector2f acceleration;
        for (std::vector<SpaceObject*>::const_iterator it = physics::getGravitySources().begin(); it != physics::getGravitySources().end(); ++it) {
            float distanceSquared = (from - (*it)->location()).lengthSquare();
            if (distanceSquared > 100.f)
                acceleration += ((*it)->location() - from) * (*it)->mass() / distanceSquared;
        }
        acceleration *= 70.f;
        Vector2f to (from + velocity * resolution + acceleration*resolution*resolution);
        velocity += acceleration*resolution;

       /* glBegin(GL_LINES);
            glVertex2f(from.x_, from.y_);
            glVertex2f(to.x_, to.y_);
        glEnd();*/

        for (std::vector<Ship*>::const_iterator it = ships::getShips().begin(); it != ships::getShips().end(); ++it) {
            if ((*it)->attackable()) {
                Vector2f shipLocation((*it)->location() + (*it)->velocity()*resolution*i*0.01f*settings::C_iDumb);
                Vector2f orthoDir(velocity.y_, -velocity.x_);
                orthoDir = orthoDir.normalize()*(*it)->radius();
                Vector2f shipLeft(shipLocation-orthoDir), shipRight(shipLocation+orthoDir);

                if (clockWise(velocity, shipRight-to) && !clockWise(velocity, shipLeft-to) && clockWise(orthoDir, from-shipRight) && !clockWise(orthoDir, to-shipRight)) {
                    if((*it)->getOwner()->team() != team) {
                       /* glPointSize(50.f);
                        glColor3f(0.f, 1.f, 0.f);
                        glBegin(GL_POINTS);
                            glVertex2f((*it)->location().x_, (*it)->location().y_);
                        glEnd();*/
                        return true;
                    }
                    else {
                      /*  glPointSize(50.f);
                        glColor3f(1.f, 0.f, 0.f);
                        glBegin(GL_POINTS);
                            glVertex2f((*it)->location().x_, (*it)->location().y_);
                        glEnd();*/
                        return false;
                    }
                }
            }
        }

        if (to.x_ < -100 || to.x_ > 1280 + 100 || to.y_ < -100 || to.y_ > 800 + 100)
            return false;

        for (std::vector<SpaceObject*>::const_iterator it = physics::getGravitySources().begin(); it != physics::getGravitySources().end(); ++it) {
            if((*it)->type() != spaceObjects::oBlackHole && ((*it)->location()-to).lengthSquare() < std::pow((*it)->radius(), 2))
                return false;
        }

        from = to;
    }

    return false;
}


