/* Ball.cpp

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

# include "SpaceObjects/Ball.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Media/sound.hpp"
# include "Particles/particles.hpp"
# include "Shaders/postFX.hpp"

# include <cmath>

Ball::Ball(Vector2f const& location):
           MobileSpaceObject(spaceObjects::oBall, location, 15.f, 7.f),
           rotation_(0),
           rotateSpeed_(0.f),
           frozen_(0.f),
           sticky_(true),
           visible_(true),
           respawnLocation_(location),
           respawnRotation_(0),
           heatTimer_(0.f),
           smokeTimer_(0.f),
           respawnTimer_(0.f) {
    physics::addMobileObject(this);
}

void Ball::update() {
    float time = timer::frameTime();

    if (visible_) {

        physics::collide(this, STATICS | MOBILES);

        if(frozen_ <= 0.f) {

            if (sticky_)
                velocity_ = Vector2f();
            else {
                Vector2f acceleration(physics::attract(this));
                // s = s0 + v0*t + 0.5*a*t*t
                location_ += velocity_*time + acceleration*0.5*time*time;
                // v = v0 + a*t
                velocity_ += acceleration*time + velocity_*(-0.2)*time;
                // spin around
                fmod(rotation_+= rotateSpeed_*time*30, 360);
                rotateSpeed_ -= rotateSpeed_*time*0.2;

                if (location_.x_ < radius_) {
                    location_.x_ = radius_;
                    velocity_.x_ = -velocity_.x_*0.6;
                }
                if (location_.x_ > 1280 - radius_) {
                    location_.x_ = 1280 - radius_;
                    velocity_.x_ = -velocity_.x_*0.6;
                }
                if (location_.y_ < radius_) {
                    location_.y_ = radius_;
                    velocity_.y_ = -velocity_.y_*0.6;
                }
                if (location_.y_ > 800 - radius_) {
                    location_.y_ = 800 - radius_;
                    velocity_.y_ = -velocity_.y_*0.6;
                }
            }

            if (frozen_ < 0.f) {
                frozen_ = 0.f;
                mass_ = 7.f;
                particles::spawnMultiple(10, particles::pCrushedIce, location_);
            }

            // heating of ball
            if (heatTimer_ > 0.f) {
                heatTimer_ -= time;
                if (smokeTimer_ > 0)
                    smokeTimer_ -= time;
                else {
                    smokeTimer_ = 0.3f/(settings::C_globalParticleCount*heatTimer_);
                    for (int i=0; i<5; ++i) {
                        particles::spawn(particles::pSmoke, location_+Vector2f::randDirLen()*radius_, velocity_);
                    }
                }
            }
        }
        else {
            velocity_ = Vector2f();
            frozen_ -= timer::frameTime()*3.f;
        }
    }
    else {
        respawnTimer_ -= time;
        if (respawnTimer_ < 0.2f && (respawnTimer_ + time) >= 0.2f)
            sound::playSound(sound::BallRespawn);
        if (respawnTimer_ <= 0)
            respawn();
    }
}

void Ball::draw() const {
    if (visible_) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0);
        glRotatef(rotation_, 0, 0, 1);

        glEnable(GL_TEXTURE_2D);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float drawRadius = radius_*(3.0/2.0);

        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ball));
        // pattern
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0.5f); glVertex2f(-drawRadius, -drawRadius);
            glTexCoord2f(0.5f, 0.5f); glVertex2f(-drawRadius,  drawRadius);
            glTexCoord2f(0.5f, 0); glVertex2f( drawRadius,  drawRadius);
            glTexCoord2f(0, 0); glVertex2f( drawRadius, -drawRadius);
        glEnd();

        if (heatTimer_ > 0) {
            // hotness
            glColor4f(1,1,1, 0.05*heatTimer_);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0.5f); glVertex2f(-drawRadius, -drawRadius);
                glTexCoord2f(0, 1.f); glVertex2f(-drawRadius,  drawRadius);
                glTexCoord2f(0.5f, 1.f); glVertex2f( drawRadius,  drawRadius);
                glTexCoord2f(0.5f, 0.5f); glVertex2f( drawRadius, -drawRadius);
            glEnd();
        }

        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0);
        // shading
        glColor4f(1,1,1, 1-0.05*heatTimer_);
        glBegin(GL_QUADS);
            glTexCoord2f(0.5f, 0); glVertex2f(-drawRadius, -drawRadius);
            glTexCoord2f(0.5f, 0.5f); glVertex2f(-drawRadius,  drawRadius);
            glTexCoord2f(1, 0.5f); glVertex2f( drawRadius,  drawRadius);
            glTexCoord2f(1, 0); glVertex2f( drawRadius, -drawRadius);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glPopMatrix();
    }
}

void Ball::onCollision(SpaceObject* with, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity) {
    sticky_ = false;
    float strength = velocity.length();

    setDamageSource(with->damageSource());

    float unfreeze(0);

    switch (with->type()) {
        case spaceObjects::oSun:
            rotateSpeed_ = rotateSpeed_*0.5 + ((velocity_.x_*direction.y_ - direction.x_*velocity_.y_)*0.1)*0.5;
            heatTimer_ = 20;
            if (strength > 50)
                sound::playSound(sound::BallPlanetCollide, location, (strength-50)/3);
            unfreeze = 40.f;
            break;

        case spaceObjects::oShip:
            setDamageSource(with->damageSource());
            particles::spawnMultiple(1, particles::pSpark, location, direction, velocity_, Color3f(0.3f, 0.3f, 0.3f));
            if (strength > 50) {
                sound::playSound(sound::ShipBallCollide, location, (strength-50)/3);
            }
            break;

        case spaceObjects::oPlanet:
            rotateSpeed_ = rotateSpeed_*0.5 + ((velocity_.x_*direction.y_ - direction.x_*velocity_.y_)*0.1)*0.5;
            if (strength > 50)
                sound::playSound(sound::BallPlanetCollide, location, (strength-50)/3);
            unfreeze = 10.f;
            break;

        case spaceObjects::oHome:
            explode();
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoAFK47:
            setDamageSource(with->damageSource());
            particles::spawnMultiple(1, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.3f, velocity_, Color3f(0.3f, 0.3f, 0.3f));
            unfreeze = 1.f;
            break;

        case spaceObjects::oAmmoROFLE:
            setDamageSource(with->damageSource());
            particles::spawnMultiple(10, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.5f, velocity_, Color3f(0.3f, 0.3f, 0.3f));
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoShotgun:
            setDamageSource(with->damageSource());
            particles::spawnMultiple(1, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.7f, velocity_, Color3f(0.3f, 0.3f, 0.3f));
            unfreeze = 1.f;
            break;

        case spaceObjects::oAmmoFlubba:
            setDamageSource(with->damageSource());
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoBurner:
            if (heatTimer_ < 20.f) heatTimer_ += 0.01f;
            velocity_ += velocity*0.03f*timer::frameTime();
            setDamageSource(with->damageSource());
            unfreeze = 1.f;
            break;

        default:;
    }

    if (frozen_ > 0)
        frozen_ -= unfreeze;
}

void Ball::onShockWave(SpaceObject* source, float intensity) {
    sticky_ = false;
    setDamageSource(source->damageSource());
}

void Ball::explode() {
    sound::playSound(sound::BallExplode, location_, 100.f);
    physics::causeShockWave(damageSource(), location_, 50.f, 300.f, 10.f);
    particles::spawnMultiple(5 , particles::pFragment, location_, location_, location_, Color3f(0.3f, 0.3f, 0.3f));
    particles::spawnMultiple(70, particles::pDust, location_);
    particles::spawnMultiple(20, particles::pExplode, location_);
    particles::spawnMultiple(8, particles::pBurningFragment, location_);
    particles::spawnMultiple(1, particles::pMiniFlame, location_);
    postFX::   onExplosion();
    location_ = respawnLocation_;
    rotation_ = respawnRotation_;
    velocity_ = Vector2f();
    rotateSpeed_ = 0.f;
    physics::removeMobileObject(this);
    visible_ = false;
    heatTimer_ = 0.f;
    respawnTimer_ = 10.f;
}

void Ball::respawn() {
    physics::addMobileObject(this);
    visible_ = true;
    sticky_  = true;
    sound::playSound(sound::BallRespawn, location_, 100.f);
}
