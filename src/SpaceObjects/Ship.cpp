/* Ship.cpp

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

# include "SpaceObjects/Ship.hpp"

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "Weapons/weapons.hpp"
# include "Specials/specials.hpp"
# include "Media/sound.hpp"
# include "Hud/hud.hpp"
# include "Media/text.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include "Controllers/Controller.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/Ball.hpp"
# include "Media/announcer.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Shaders/postFX.hpp"
# include "Items/items.hpp"

# include <cmath>
# include <sstream>

Ship::Ship(Vector2f const& location, float rotation, Player* owner):
               MobileSpaceObject(spaceObjects::oShip, location, 16.f, 10.f),
               owner_(owner),
               rotation_(rotation),
               rotateSpeed_(1.f),
               up_(false), left_(false), right_(false),
               docked_(true),
               weaponChange_(false),
               specialChange_(false),
               visible_(true),
               respawnTimer_(0.f),
               damageSourceResetTimer_(0.f),
               respawnLocation_(location),
               respawnRotation_(rotation),
               currentWeapon_(new AFK47(this)),
               currentSpecial_(new Heal(this)),
               life_(200.f),
               maxLife_(life_),
               fuel_(100.f),
               maxFuel_(fuel_),
               collectedPowerUps_(items::COUNT, NULL),
               fragStars_(0),
               damageByLocalPlayer_(0.f),
               damageCheckTimer_(0.f),
               damageDirection_(0.f, 0.f),
               collisionCount_(0) {

    decoObjects::addName(this);

    if  ((owner_->controlType_ == controllers::cPlayer1) | (owner_->controlType_ == controllers::cPlayer2))
        decoObjects::addHighlight(this);
    else {
        life_ = 50 + static_cast<float>(settings::C_iDumb)*2.5f;
        maxLife_ = life_;
    }

    physics::addMobileObject(this);
    owner->ship_ = this;
    damageSource_ = owner_;
}

void Ship::update() {
    float time = timer::frameTime();

    if (damageSourceResetTimer_ > 0.f) {
        damageSourceResetTimer_ -= time;
        if (damageSourceResetTimer_ <= 0.f)
            damageSource_ = owner_;
    }

    if (damageCheckTimer_ > 0.f) {
        damageCheckTimer_ -= time;
        if (damageCheckTimer_ <= 0.f) {
            float damage(damageByLocalPlayer_*20.f);
            if (std::abs(damage) >= 1.f) {
                particles::spawn(particles::pNumber, location_+Vector2f(0.f, -20.f), Vector2f(damage, 20.f + std::abs(damage)*0.02f), (damageDirection_/collisionCount_+velocity_)*0.5f);
                damageDirection_ = Vector2f();
                damageByLocalPlayer_ = 0;
                collisionCount_ = 0;
            }
        }
    }

    if (visible_) {
        // spin around
        if (collectedPowerUps_[items::puReverse]) {
            if (right_) fmod(rotation_-= rotateSpeed_*time*30.f, 360.f);
            if (left_ ) fmod(rotation_+= rotateSpeed_*time*30.f, 360.f);
        }
        else {
            if (right_) fmod(rotation_+= rotateSpeed_*time*30.f, 360.f);
            if (left_ ) fmod(rotation_-= rotateSpeed_*time*30.f, 360.f);
        }
        if (!right_ && !left_) rotateSpeed_ = 1.0;
        else if (rotateSpeed_ < 10.f) rotateSpeed_ += time*30.f;

        // accelerate
        float angleRad = rotation_ * M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        Vector2f acceleration;
        if (up_ && getFuel() > 0.f) {
            fuel_ -= time*3.f;
            acceleration = faceDirection * 300.f;
            particles::spawnTimed(150.f/settings::C_globalParticleCount, particles::pFuel, location_-faceDirection*radius_, faceDirection, velocity_);
            particles::spawnTimed(10.f/settings::C_globalParticleCount, particles::pHeatJet, location_-faceDirection*radius_*1.5f, faceDirection, velocity_);
        }
        else {
            acceleration = Vector2f();
            if (getFuel() < maxFuel_)
                fuel_ += time*0.5f;
            else
                fuel_ = maxFuel_;
        }

        // movement
        // check if docked
        Home const* home = owner_->team()->home();
        Vector2f toHome = home->location()-location_;
        bool closeToHome(toHome.lengthSquare() < std::pow(home->radius() + radius_ + 0.1f, 2.f));
        if (!up_ && velocity_.lengthSquare() < 10000.f && closeToHome && ((faceDirection + toHome.normalize()).lengthSquare() < 0.16f)) {
            docked_ = true;
            velocity_ = Vector2f();
            if (fuel_ < maxFuel_) fuel_ += time*maxFuel_*0.2; else fuel_ = maxFuel_;
            if (life_ < maxLife_) life_ += time*maxLife_*0.2; else life_ = maxLife_;

            if (owner_->controlType_ == controllers::cPlayer1 || owner_->controlType_ == controllers::cPlayer2) {
                if (life_ < maxLife_) {
                    damageByLocalPlayer_ += time*maxLife_*0.2;
                    collisionCount_ = 1;
                    damageDirection_ = Vector2f(0.f, -250.f);
                }
                if (damageCheckTimer_ <= 0.f)
                    damageCheckTimer_ = 0.6f;
            }
        }
        else {
            docked_ = false;
            weaponChange_ = false;
            specialChange_ = false;
            acceleration += physics::attract(this);
        }

        // s = s0 + v0*t + 0.5*a*t*t
        location_ += velocity_*time + acceleration*0.5f*time*time;
        // v = v0 + a*t
        velocity_ += acceleration*time + velocity_*(-0.2f)*time;

        physics::collide(this, STATICS | MOBILES);

        if (location_.x_ < radius_) {
            location_.x_ = radius_;
            velocity_.x_ = 0.f;
        }
        if (location_.x_ > 1280.f - radius_) {
            location_.x_ = 1280.f - radius_;
            velocity_.x_ = 0.f;
        }
        if (location_.y_ < radius_) {
            location_.y_ = radius_;
            velocity_.y_ = 0.f;
        }
        if (location_.y_ > 800.f - radius_) {
            location_.y_ = 800.f - radius_;
            velocity_.y_ = 0.f;
        }

        // check for death
        if (getLife() <= 0) explode();
    }
    else {
        respawnTimer_ -= time;
        if (respawnTimer_ < 0) respawn();
    }
}

void Ship::draw() const {
    if (visible_) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // draw glow
        Color3f tmp = owner_->team_->color();
        if (tmp.v() < 0.4f) tmp.v(0.4f);
        if (tmp.s() < 0.5f) tmp.s(0.5f);
        tmp.gl4f(0.5f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.75f);         glVertex2f(-radius_*3.6f,-radius_*3.6f);
            glTexCoord2f(0.f, 0.875f);      glVertex2f(-radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.875f);   glVertex2f( radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.75f);      glVertex2f( radius_*3.6f,-radius_*3.6f);
        glEnd();

        // draw ship
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRotatef(rotation_, 0.f, 0.f, 1.f);

        float x, y;

        x = static_cast<float>(owner_->graphic()%8)*0.125f;
        y = static_cast<float>(std::floor(owner_->graphic()*0.125f))*0.375f;

        glColor3f(1.f, 1.f, 1.f);
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-radius_, -radius_);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-radius_,  radius_);
            glTexCoord2f(x+0.125f, y);          glVertex2f( radius_,  radius_);
            glTexCoord2f(x, y);                 glVertex2f( radius_, -radius_);
        glEnd();

        y += 0.125f;

        owner_->team()->color().gl3f();
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-radius_, -radius_);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-radius_,  radius_);
            glTexCoord2f(x+0.125f, y);          glVertex2f( radius_,  radius_);
            glTexCoord2f(x, y);                 glVertex2f( radius_, -radius_);
        glEnd();

        y += 0.125f;

        owner_->color().gl3f();
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-radius_, -radius_);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-radius_,  radius_);
            glTexCoord2f(x+0.125f, y);          glVertex2f( radius_,  radius_);
            glTexCoord2f(x, y);                 glVertex2f( radius_, -radius_);
        glEnd();

        glPopMatrix();
    }
    else if (respawnTimer_ > 6.f) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        // draw glow
        owner_->team_->color().gl4f((respawnTimer_ - 6.f)*0.25f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.75f);         glVertex2f(-radius_*3.6f,-radius_*3.6f);
            glTexCoord2f(0.f, 0.875f);      glVertex2f(-radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.875f);   glVertex2f( radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.75f);      glVertex2f( radius_*3.6f,-radius_*3.6f);
        glEnd();

        glPopMatrix();
    }
}

void Ship::drawWeapon() const {
     if (visible_) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0.f);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRotatef(rotation_, 0.f, 0.f, 1.f);

        // draw weapon
        currentWeapon_->draw();

        // draw special
        currentSpecial_->draw();

        glPopMatrix();
     }
}

void Ship::onCollision(SpaceObject* with, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();
    // damage
    float amount(0.f);
    float waitForOtherDamage(0.001f);

    switch (with->type()) {
        case spaceObjects::oSun:
            amount = strength*0.08f + 20;
            if (strength > 50) sound::playSound(sound::BallPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oShip:
            setDamageSource(with->damageSource());
            amount = strength*0.01f;
            dynamic_cast<Ship*>(with)->setDamageSource(damageSource_);
            if (strength > 50) sound::playSound(sound::ShipCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oPlanet:
            if (strength > 75) amount = strength*0.08f;
            if (strength > 50) sound::playSound(sound::ShipPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oHome:
            if (strength > 150) amount = strength*0.06f;
            if (strength > 50) sound::playSound(sound::ShipPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oBall:
            amount =  dynamic_cast<Ball*>(with)->heatAmount()*0.1f;
            particles::spawnMultiple(2, particles::pSpark, location, direction*100.f, velocity_, owner_->color());
            if (strength > 50) sound::playSound(sound::ShipPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oAmmoAFK47:
            amount = strength*0.001f;
            waitForOtherDamage = 0.15f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.3f, velocity_, owner_->color());
            break;

        case spaceObjects::oAmmoROFLE:
            amount = strength*0.04f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(20, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.5f, velocity_, owner_->color());
            break;

        case spaceObjects::oAmmoShotgun:
            amount = strength*0.002f;
            waitForOtherDamage = 0.1f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.7f, velocity_, owner_->color());
            break;

        case spaceObjects::oAmmoFlubba:
            amount = strength*0.1f+3.f;
            setDamageSource(with->damageSource());
            break;

        case spaceObjects::oCannonBall:
            amount = life_;
            setDamageSource(owner_);
            break;

        case spaceObjects::oAmmoBurner:
            amount = timer::frameTime()*0.5f;
            waitForOtherDamage = 0.15f;
            velocity_ += velocity*0.03f*timer::frameTime();
            // chance to spawn smoke
            if (sf::Randomizer::Random(0.f, 100.f)/settings::C_globalParticleCount < 0.01f) particles::spawn(particles::pSmoke, location, velocity);
            setDamageSource(with->damageSource());
            break;

        case spaceObjects::oAmmoRocket:
            amount = 20.f;
            setDamageSource(with->damageSource());
            break;

        case spaceObjects::oAmmoFist:
            amount = 25.f+sf::Randomizer::Random(-3.f, 3.f);
            setDamageSource(with->damageSource());
            break;

        default:;
    }
    if (!collectedPowerUps_[items::puShield]) {
        life_ -= amount;
         if ((damageSource_ && (damageSource_->controlType_ == controllers::cPlayer1 || damageSource_->controlType_ == controllers::cPlayer2))
            || owner_->controlType_ == controllers::cPlayer1 ||  owner_->controlType_ == controllers::cPlayer2) {
                if (damageCheckTimer_ <= 0.f)
                    damageCheckTimer_ = waitForOtherDamage;
                damageByLocalPlayer_ -= amount;
                MobileSpaceObject* target = dynamic_cast<MobileSpaceObject*>(with);
                if (target)
                    damageDirection_ += target->velocity();
                else
                    damageDirection_ = Vector2f();
                ++collisionCount_;
        }
    }
}

void Ship::onShockWave(Player* damageSource, float intensity) {
    setDamageSource(damageSource);
    if (!collectedPowerUps_[items::puShield]) {
        float damage(intensity*(20.f + settings::C_iDumb));
        life_ -= damage;
        if ((damageSource_ && (damageSource_->controlType_ == controllers::cPlayer1 || damageSource_->controlType_ == controllers::cPlayer2))
            || owner_->controlType_ == controllers::cPlayer1 ||  owner_->controlType_ == controllers::cPlayer2) {
            damageByLocalPlayer_ -= damage;
            ++collisionCount_;
            if (damageCheckTimer_ <= 0.f)
                damageCheckTimer_ = 0.01f;
        }
    }
}

void Ship::setDamageSource(Player* evilOne) {
    damageSource_ = evilOne;
    damageSourceResetTimer_ = 1.5f;
}

void Ship::heal(Player* source, int amount) {
    float lifeAmount((maxLife_/100.f)*amount);
    if (life_ + lifeAmount > maxLife_)
        lifeAmount = maxLife_-life_;
    life_+=lifeAmount;

    if (source->controlType_ == controllers::cPlayer1 || source->controlType_ == controllers::cPlayer2
            || owner_->controlType_ == controllers::cPlayer1 ||  owner_->controlType_ == controllers::cPlayer2) {
        damageByLocalPlayer_ += lifeAmount;
        ++collisionCount_;
        if (damageCheckTimer_ <= 0.f)
            damageCheckTimer_ = 0.01f;
    }
}

void Ship::refuel(Player* source, int amount) {
    float fuelAmount((maxFuel_/100.f)*amount);
    (fuel_ + fuelAmount) > maxFuel_ ? fuel_ = maxFuel_ : fuel_ += fuelAmount;
}

float Ship::getLife() const {
    return life_ < 0.f ? 0.f : life_/maxLife_*100.f;
}

float Ship::getFuel() const {
    return fuel_ < 0.f ? 0.f : fuel_/maxFuel_*100.f;
}

Player* Ship::getOwner() const {
    return owner_;
}

std::vector<PowerUp*> const& Ship::getCollectedPowerUps() const {
    return collectedPowerUps_;
}

void Ship::explode() {
    sound::playSound(sound::ShipExplode, location_, 100.f);
    particles::spawnMultiple(5 , particles::pFragment, location_, location_, location_, owner_->color());
    particles::spawnMultiple(50, particles::pDust, location_);
    particles::spawnMultiple(20, particles::pExplode, location_);
    particles::spawnMultiple(5, particles::pBurningFragment, location_);
    particles::spawnMultiple(1, particles::pMiniFlame, location_);
    physics::  causeShockWave(damageSource(), location_, 250.f, 400.f);
    particles::spawn(particles::pShockWave, location_);
    physics::  removeMobileObject(this);
    timer::    onShipExplode();
    postFX::   onExplosion();
    visible_ = false;
    life_ = 0.f;
    fuel_ = 0.f;
    respawnTimer_ = 10.f;

    ++owner_->deaths_;

    if (!damageSource_) damageSource_ = owner_;

    if (damageSource_ == owner_) {
        ++owner_->suicides_;
        --owner_->points_;
        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            --damageSource_->team()->points_;

        announcer::announce(announcer::Affronting);
    }
    else if (damageSource_->team() == owner_->team()) {
        ++damageSource_->teamKills_;
        --damageSource_->points_;

        (damageSource_->ship()->fragStars_-1 < 0) ? damageSource_->ship()->fragStars_=0 : --damageSource_->ship()->fragStars_;

        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            --damageSource_->team()->points_;

        announcer::announce(announcer::Affronting);
    }
    else {
        ++damageSource_->frags_;
        ++damageSource_->points_;

        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            ++damageSource_->team()->points_;

        ++damageSource_->ship()->fragStars_;

        announcer::announce(announcer::Praising);
    }

}

void Ship::respawn() {
    physics::addMobileObject(this);
    location_ = respawnLocation_;
    rotation_ = respawnRotation_;
    velocity_ = Vector2f();
    rotateSpeed_ = (1.f);
    life_ = maxLife_;
    fuel_ = maxFuel_;
    fragStars_ = 0;
    visible_ = true;
    damageByLocalPlayer_ = 0.f;
    damageDirection_ = Vector2f();
    collisionCount_ = 0;
    sound::playSound(sound::ShipRespawn, location_, 100.f);
}

