/* Ship.cpp

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
# include "Teams/Team.hpp"
# include "Games/games.hpp"
# include "Controllers/Controller.hpp"
# include "SpaceObjects/Ball.hpp"
# include "Media/announcer.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Shaders/postFX.hpp"
# include "Items/items.hpp"
# include "SpaceObjects/Home.hpp"
# include "defines.hpp"

# include <cmath>
# include <sstream>

Ship::Ship(Vector2f const& location, float rotation, Player* owner):
               MobileSpaceObject(spaceObjects::oShip, location, SHIP_RADIUS, 10.f),
               owner_(owner),
               rotation_(rotation),
               rotateSpeed_(1.f),
               up_(0), left_(0), right_(0),
               docked_(true),
               weaponChange_(false),
               specialChange_(false),
               visible_(true),
               ghostTimer_(1.f),
               frozen_(0.f),
               respawnTimer_(0.f),
               damageSourceResetTimer_(0.f),
               respawnLocation_(location),
               respawnRotation_(rotation),
               currentWeapon_(NULL),
               currentSpecial_(NULL),
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

    if  (owner_->controlType_ == controllers::cPlayer1) {
        decoObjects::addHighlight(this);
        currentWeapon_  = weapons:: create(settings::C_playerIWeapon, this);
        currentSpecial_ = specials::create(settings::C_playerISpecial, this);
    }
    else if  (owner_->controlType_ == controllers::cPlayer2) {
        decoObjects::addHighlight(this);
        currentWeapon_  = weapons:: create(settings::C_playerIIWeapon, this);
        currentSpecial_ = specials::create(settings::C_playerIISpecial, this);
    }
    else {
        //life_ = 25.f + static_cast<float>(settings::C_iDumb)*1.75f;
        //maxLife_ = life_;
        currentWeapon_  = weapons:: create(weapons::wAFK47, this);
        currentSpecial_ = specials::create(specials::sHeal, this);
    }

    owner->ship_ = this;
    damageSource_ = owner_;
}

void Ship::update() {
    float time = timer::frameTime();

    if (damageSourceResetTimer_ > 0.f) {
        damageSourceResetTimer_ -= time;
        if (frozen_ <= 0.f && damageSourceResetTimer_ <= 0.f)
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
        if (ghostTimer_ > 0.f && !(docked_ && ghostTimer_ == 1.f)) {
            ghostTimer_ -= time;
            if (ghostTimer_ <= 0.f)
                physics::addMobileObject(this);
        }

        if ((games::elapsedTime() > 4.f) || (games::type() == games::gTutorial)) {

            if (frozen_ <= 0) {
                // spin around
                if (collectedPowerUps_[items::puReverse]) {
                    if (right_ > 15) fmod(rotation_-= rotateSpeed_*time*0.3f*right_, 360.f);
                    if (left_  > 15) fmod(rotation_+= rotateSpeed_*time*0.3f*left_, 360.f);
                }
                else {
                    if (right_ > 15) fmod(rotation_+= rotateSpeed_*time*0.3f*right_, 360.f);
                    if (left_  > 15) fmod(rotation_-= rotateSpeed_*time*0.3f*left_, 360.f);
                }
                if (right_ == 0 && left_ == 0) rotateSpeed_ = 1.0;
                else if (rotateSpeed_ < 10.f) rotateSpeed_ += time*30.f;

                // accelerate
                float angleRad = rotation_ * M_PI / 180.f;
                Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
                Vector2f acceleration;
                if (up_ > 15 && getFuel() > 0.f) {
                    fuel_ -= time*0.03f * up_;
                    acceleration = faceDirection * 3.f * up_;
                    particles::spawnTimed(1.5f/settings::C_globalParticleCount*up_, particles::pFuel, location_-faceDirection*radius_, faceDirection, velocity_);
                    particles::spawnTimed(0.1f/settings::C_globalParticleCount*up_, particles::pHeatJet, location_-faceDirection*radius_*1.5f, faceDirection, velocity_);
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
                if (up_ < 10 && velocity_.lengthSquare() < 13000.f && closeToHome && ((faceDirection + toHome.normalize()).lengthSquare() < 0.26f)) {
                    docked_ = true;
                    velocity_ = Vector2f();
                    if (fuel_ < maxFuel_) {
                        if (fuel_ > 0) fuel_ += time*maxFuel_*0.2;
                    }
                    else fuel_ = maxFuel_;

                    if (life_ < maxLife_) {
                        if (life_ > 0) life_ += time*maxLife_*0.2;
                    }
                    else life_ = maxLife_;

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

                if (ghostTimer_ <= 0.f)
                    physics::collide(this, STATICS | MOBILES);
                else
                    physics::collide(this, STATICS);

                if (location_.x_ < radius_) {
                    location_.x_ = radius_;
                    velocity_.x_ = 0.f;
                }
                if (location_.x_ > SPACE_X_RESOLUTION - radius_) {
                    location_.x_ = SPACE_X_RESOLUTION - radius_;
                    velocity_.x_ = 0.f;
                }
                if (location_.y_ < radius_) {
                    location_.y_ = radius_;
                    velocity_.y_ = 0.f;
                }
                if (location_.y_ > SPACE_Y_RESOLUTION - radius_) {
                    location_.y_ = SPACE_Y_RESOLUTION - radius_;
                    velocity_.y_ = 0.f;
                }

            }
            else {
                frozen_ -= timer::frameTime()*3.f;
                life_ -= timer::frameTime()*7.f;

                if (frozen_ <= 0.f) {
                    frozen_ = 0.f;
                    mass_ = 10.f;
                    particles::spawnMultiple(2, particles::pCrushedIce, location_);
                }

                velocity_ = Vector2f();
                if(damageSource_==players::getPlayerI() || damageSource_==players::getPlayerII() || owner_==players::getPlayerI() || owner_==players::getPlayerII()) {
                    damageByLocalPlayer_ -= timer::frameTime()*10.f;
                    collisionCount_ = 1;
                    damageDirection_ = Vector2f(0.f, 400.f);
                    if (damageCheckTimer_ <= 0.f)
                        damageCheckTimer_ = 1.f;
                }
            }

            // check for death
            if (getLife() <= 0) explode();

        }
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

        // draw ship
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRotatef(rotation_, 0.f, 0.f, 1.f);

        float x, y, alpha(ghostTimer_ == 1.f ? 0.2f*std::sin(timer::totalTime()*8.f + 1.5f*M_PI)+0.4f : (ghostTimer_ > 0.f ? ghostTimer_*(0.2f*std::sin(timer::totalTime()*8.f + 1.5f*M_PI)+0.4f) + 1.f-ghostTimer_ : 1.f));

        x = static_cast<float>(owner_->graphic()%8)*0.125f;
        y = static_cast<float>(std::floor(owner_->graphic()*0.125f))*0.375f;

        glColor4f(1.f, 1.f, 1.f, alpha);
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-radius_, -radius_);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-radius_,  radius_);
            glTexCoord2f(x+0.125f, y);          glVertex2f( radius_,  radius_);
            glTexCoord2f(x, y);                 glVertex2f( radius_, -radius_);
        glEnd();

        y += 0.125f;

        owner_->team()->color().gl4f(alpha);
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-radius_, -radius_);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-radius_,  radius_);
            glTexCoord2f(x+0.125f, y);          glVertex2f( radius_,  radius_);
            glTexCoord2f(x, y);                 glVertex2f( radius_, -radius_);
        glEnd();

        y += 0.125f;

        owner_->color().gl4f(alpha);
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
        glRotatef(timer::totalTime()*-50, 0.f, 0.f, 1.f);

        float alpha(ghostTimer_ == 1.f ? 0.2f*std::sin(timer::totalTime()*8.f + 1.5f*M_PI)+0.4f : (ghostTimer_ > 0.f ? ghostTimer_*(0.2f*std::sin(timer::totalTime()*8.f + 1.5f*M_PI)+0.4f) + 1.f-ghostTimer_ : 1.f));

        // draw special
        currentSpecial_->draw(alpha);

        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0.f);
        glRotatef(rotation_, 0.f, 0.f, 1.f);

        // draw weapon
        currentWeapon_->draw(alpha);

        glPopMatrix();
     }
}

void Ship::onCollision(SpaceObject* with, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();
    // damage
    float amount(0.f);
    float waitForOtherDamage(0.001f);

    float unfreeze(0);

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
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoAFK47:
            amount = strength*0.002f;
            waitForOtherDamage = 0.15f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.3f, velocity_, owner_->color());
            unfreeze = 0.1f;
            break;

        case spaceObjects::oAmmoROFLE:
            amount = strength*0.04f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(20, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.5f, velocity_, owner_->color());
            unfreeze = 20.f;
            break;

        case spaceObjects::oAmmoShotgun:
            amount = strength*0.0015f;
            waitForOtherDamage = 0.1f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.7f, velocity_, owner_->color());
            unfreeze = 0.1f;
            break;

        case spaceObjects::oAmmoFlubba:
            amount = sf::Randomizer::Random(2.5f, 3.f);
            setDamageSource(with->damageSource());
            unfreeze = 4.f;
            break;

        case spaceObjects::oMiniAmmoFlubba:
            amount = sf::Randomizer::Random(0.7f, 1.f);
            waitForOtherDamage = 0.3f;
            setDamageSource(with->damageSource());
            break;

        case spaceObjects::oCannonBall:
            amount = life_;
            setDamageSource(owner_);
            unfreeze = frozen_;
            break;

        case spaceObjects::oAmmoBurner:
            amount = timer::frameTime()*0.8f;
            waitForOtherDamage = 0.15f;
            if (frozen_ <= 0) velocity_ += velocity*0.03f*timer::frameTime();
            // chance to spawn smoke
            if (sf::Randomizer::Random(0.f, 100.f)/settings::C_globalParticleCount < 0.01f) particles::spawn(particles::pSmoke, location, velocity);
            setDamageSource(with->damageSource());
            unfreeze = 0.05f;
            break;

        case spaceObjects::oAmmoRocket:
            amount = 10.f;
            setDamageSource(with->damageSource());
            unfreeze = 10.f;
            break;

        case spaceObjects::oAmmoFist:
            amount = 18.f+sf::Randomizer::Random(-3.f, 3.f);
            setDamageSource(with->damageSource());
            unfreeze = 15.f;
            break;

        case spaceObjects::oAmmoInsta:
            amount = life_;
            setDamageSource(with->damageSource());
            break;

        default:;
    }

    if (frozen_ > 0) {
        frozen_ -= unfreeze;
        if (frozen_ <= 0.f) {
            frozen_ = 0.f;
            mass_ = 10.f;
            particles::spawnMultiple(2, particles::pCrushedIce, location_);
        }
    }

    if (attackable()) {
        // increase the amount done to weak bots
        // strong bots just take normal damage
        if (damageSource_
            && (damageSource_->controlType_ == controllers::cPlayer1 || damageSource_->controlType_ == controllers::cPlayer2)
            && (owner_->controlType_ != controllers::cPlayer1 && owner_->controlType_ != controllers::cPlayer2)
            && amount < life_)
            amount *= (10.f - 0.09f*settings::C_iDumb);

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
    if (frozen_ > 0)
        frozen_-=10;

    else {
        setDamageSource(damageSource);
        if (!collectedPowerUps_[items::puShield]) {
            float damage(intensity*0.1f*(20.f + settings::C_iDumb));
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
}

void Ship::setDamageSource(Player* evilOne) {
    if(frozen_<=0.f) {
        damageSource_ = evilOne;
        damageSourceResetTimer_ = 1.5f;
    }
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
    physics::  causeShockWave(damageSource(), location_, 400.f, 200.f, 2.f);
    particles::spawn(particles::pShockWave, location_);
    physics::  removeMobileObject(this);
    timer::    onShipExplode();
    postFX::   onExplosion();
    visible_ = false;
    life_ = 0.f;
    fuel_ = 0.f;
    if (games::type() == games::gGraveItation)
        respawnTimer_ = 2.f;
    else
        respawnTimer_ = 5.f;
    frozen_ = 0.f;
    currentSpecial_->stop();

    ++owner_->deaths_;

    if (!damageSource_) damageSource_ = owner_;

    if (damageSource_ == owner_) {
        ++owner_->suicides_;
        --owner_->points_;
        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            damageSource_->team()->subtractPoint();

        announcer::announce(announcer::Affronting);
    }
    else if (damageSource_->team() == owner_->team()) {
        ++damageSource_->teamKills_;
        --damageSource_->points_;

        (damageSource_->ship()->fragStars_-1 < 0) ? damageSource_->ship()->fragStars_=0 : --damageSource_->ship()->fragStars_;

        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            damageSource_->team()->subtractPoint();

        announcer::announce(announcer::Affronting);
    }
    else {
        ++damageSource_->frags_;
        ++damageSource_->points_;

        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            damageSource_->team()->addPoint();

        ++damageSource_->ship()->fragStars_;

        announcer::announce(announcer::Praising);
    }

}

void Ship::respawn() {
    location_ = respawnLocation_;
    rotation_ = respawnRotation_;
    velocity_ = Vector2f();
    rotateSpeed_ = 1.f;
    life_ = maxLife_;
    fuel_ = maxFuel_;
    fragStars_ = 0;
    ghostTimer_ = 1.f;
    visible_ = true;
    docked_ = true;
    damageByLocalPlayer_ = 0.f;
    damageDirection_ = Vector2f();
    collisionCount_ = 0;
    sound::playSound(sound::ShipRespawn, location_, 100.f);
}

float Ship::rotation() const {
    return rotation_;
}

bool Ship::collidable() const {
    return visible_ && ghostTimer_ <= 0.f;
}

bool Ship::attackable() const {
    return collidable() && frozen_ <= 0 && !collectedPowerUps_[items::puShield];
}
