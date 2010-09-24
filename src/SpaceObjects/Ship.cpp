/* Ship.cpp

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
# include "Media/sound.hpp"
# include "Hud/hud.hpp"
# include "Players/Player.hpp"
# include "Players/Team.hpp"
# include "Controllers/Controller.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/Ball.hpp"
# include "Media/announcer.hpp"

# include <cmath>
# include <sstream>

Ship::Ship(Vector2f const& location, float rotation, Player* owner):
               MobileSpaceObject(spaceObjects::oShip, location, 12.f, 10.f),
               owner_(owner),
               rotation_(rotation),
               rotateSpeed_(1.f),
               up_(false), left_(false), right_(false),
               docked_(true),
               weaponChange_(false),
               visible_(true),
               respawnTimer_(0.f),
               damageSourceResetTimer_(0.f),
               respawnLocation_(location),
               respawnRotation_(rotation),
               currentWeapon_(new AFK47(this)),
               life_(100),
               fuel_(100),
               collectedItems_(1),
               fragStars_(0),
               fragStarTimer_(0.f) {
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

    if (fragStarTimer_ > 0.f) {
        fragStarTimer_ -= time;
        if (fragStarTimer_ <= 0.f)
            fragStars_ = 0;
    }

    if (visible_) {
        // spin around
        if (right_) fmod(rotation_+= rotateSpeed_*time*30.f, 360.f);
        if (left_ ) fmod(rotation_-= rotateSpeed_*time*30.f, 360.f);
        if (!right_ && !left_) rotateSpeed_ = 1.0;
        else if (rotateSpeed_ < 10.f) rotateSpeed_ += time*30.f;

        // accelerate
        float angleRad = rotation_ * M_PI / 180.f;
        Vector2f faceDirection(std::cos(angleRad), std::sin(angleRad));
        Vector2f acceleration;
        if (up_) {
            fuel_ -= time;
            acceleration = faceDirection * 300.f;
            particles::spawnTimed(150.f/settings::C_globalParticleCount, particles::pFuel, location_-faceDirection*radius_, faceDirection, velocity_);
        }
        else acceleration = Vector2f();

        // movement
        // check if docked
        Home const* home = owner_->team()->home();
        Vector2f toHome = home->location()-location_;
        bool closeToHome(toHome.lengthSquare() < std::pow(home->radius() + radius_ + 0.1f, 2.f));
        if (!up_ && velocity_.lengthSquare() < 4000.f && closeToHome && ((faceDirection + toHome.normalize()).lengthSquare() < 0.16f)) {
            docked_ = true;
            velocity_ = Vector2f();
            if (fuel_ < 100.f) fuel_ += time*20; else fuel_ = 100.f;
            if (life_ < 100.f) life_ += time*20; else life_ = 100.f;
        }
        else {
            docked_ = false;
            weaponChange_ = false;
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
        owner_->team_->color().gl4f(0.9f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.f, 0.f);         glVertex2f(-radius_*3.6f,-radius_*3.6f);
            glTexCoord2f(0.f, 0.125f);      glVertex2f(-radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.125f);   glVertex2f( radius_*3.6f, radius_*3.6f);
            glTexCoord2f(0.125f, 0.f);      glVertex2f( radius_*3.6f,-radius_*3.6f);
        glEnd();

        // draw ship
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glRotatef(rotation_, 0.f, 0.f, 1.f);

        glDisable(GL_TEXTURE_2D);
        currentWeapon_->draw();
        glEnable(GL_TEXTURE_2D);

        float x, y;

        x = static_cast<float>(owner_->graphic()%4)*0.25f + 0.125f;
        y = static_cast<float>(std::floor(owner_->graphic()*0.25f))*0.125f;

        glColor3f(1.f, 1.f, 1.f);
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-14.f, -14.f);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-14.f,  14.f);
            glTexCoord2f(x+0.125f, y);          glVertex2f( 14.f,  14.f);
            glTexCoord2f(x, y);                 glVertex2f( 14.f, -14.f);
        glEnd();

        x -= 0.125f;

        owner_->color().gl3f();
        glBegin(GL_QUADS);
            glTexCoord2f(x, y+0.125f);          glVertex2f(-14.f, -14.f);
            glTexCoord2f(x+0.125f, y+0.125f);   glVertex2f(-14.f,  14.f);
            glTexCoord2f(x+0.125f, y);          glVertex2f( 14.f,  14.f);
            glTexCoord2f(x, y);                 glVertex2f( 14.f, -14.f);
        glEnd();

        glPopMatrix();
    }
}

void Ship::drawName() const {
    if (visible_) {
        if (weaponChange_ && ((owner_->controlType_ == controllers::cPlayer1) | (owner_->controlType_ == controllers::cPlayer2)))
            hud::drawSpaceText(currentWeapon_->getName(), location_ + Vector2f(0.f, -radius_)*2.5f);
        else {
            Color3f color(1.f, 0.f, 0.f);
            color.h(color.h() + getLife());
            if (settings::C_drawBotOrientation)
                switch (owner_->controlType_) {
                    case controllers::cAggroBot:
                        hud::drawSpaceText(owner_->name() + " [AGGRO]", location_ + Vector2f(0.f, -radius_)*2.5f,
                        font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color, velocity_);
                        break;
                    case controllers::cDefBot:
                        hud::drawSpaceText(owner_->name() + " [DEF]", location_ + Vector2f(0.f, -radius_)*2.5f,
                        font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color, velocity_);
                        break;
                    case controllers::cMidBot:
                        hud::drawSpaceText(owner_->name() + " [MID]", location_ + Vector2f(0.f, -radius_)*2.5f,
                        font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color, velocity_);
                        break;
                    default:
                        hud::drawSpaceText(owner_->name(), location_ + Vector2f(0.f, -radius_)*2.5f,
                        font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color, velocity_);
                }
            else hud::drawSpaceText(owner_->name(), location_ + Vector2f(0.f, -radius_)*2.5f,
                 font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color, velocity_);
        }

        if (fragStars_ > 0) {
            Color3f color;
            int showAmount;
            if (fragStars_ > 10) {
                color = Color3f(1.f, 0.8f, 0.f);
                showAmount = fragStars_ -10;
            }
            else if (fragStars_ > 5) {
                color = Color3f(0.8f, 0.8f, 0.8f);
                showAmount = fragStars_-5;
            }
            else {
                color = Color3f(1.0f, 0.5f, 0.7f);
                showAmount = fragStars_;
            }

            std::stringstream sstr;
            for (int i=0; i<showAmount; ++i)
                sstr << "*";
            hud::drawSpaceText(sstr.str(), location_ + Vector2f(0.f, -radius_)*2.5f + Vector2f(0.f, -10.f),
                               font::HandelGotDLig, 20.f, TEXT_ALIGN_CENTER, color, velocity_);
        }
    }
}

void Ship::drawHighLight() const {
    if (visible_ && ((owner_->controlType_ == controllers::cPlayer1) | (owner_->controlType_ == controllers::cPlayer2))) {
        glPushMatrix();
        glLoadIdentity();
        glTranslatef(location_.x_, location_.y_, 0.f);
        glRotatef(fmod(timer::totalTime()*100.f, 360.f), 0.f, 0.f, 1.f);

        // wobble when charging
        if ((docked_ && (getLife() < 100.f) | (getFuel() < 100.f)))
            glScalef(std::sin(timer::totalTime()*10.f)*0.15f + 1.f, std::sin(timer::totalTime()*10.f)*0.15f + 1.f, 0.f);

        owner_->color().gl4f(0.3f);
        glBegin(GL_QUADS);
            glTexCoord2f(0.125f, 0.f);     glVertex2f(-radius_*3.2f,-radius_*3.2f);
            glTexCoord2f(0.125f, 0.125f);  glVertex2f(-radius_*3.2f, radius_*3.2f);
            glTexCoord2f(0.25f, 0.125f);   glVertex2f( radius_*3.2f, radius_*3.2f);
            glTexCoord2f(0.25f, 0.f);      glVertex2f( radius_*3.2f,-radius_*3.2f);
        glEnd();

        glPopMatrix();
    }
}

void Ship::onCollision(SpaceObject* with, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();
    // damage
    float amount(0.f);

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
            if (strength > 75) amount = strength*0.08f;
            if (strength > 50) sound::playSound(sound::ShipPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oBall:
            amount =  dynamic_cast<Ball*>(with)->heatAmount()*0.1f;
            particles::spawnMultiple(2, particles::pSpark, location, direction*100.f, velocity_, owner_->color_);
            if (strength > 50) sound::playSound(sound::ShipPlanetCollide, location, (strength-50)/3);
            break;

        case spaceObjects::oAmmoAFK47:
            amount = strength*0.0005f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.3f, velocity_, owner_->color_);
            break;

        case spaceObjects::oAmmoROFLE:
            amount = strength*0.04f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(20, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.5f, velocity_, owner_->color_);
            break;

        case spaceObjects::oAmmoShotgun:
            amount = strength*0.003f;
            setDamageSource(with->damageSource());
            particles::spawnMultiple(2, particles::pSpark, location, dynamic_cast<MobileSpaceObject*>(with)->velocity()*0.7f, velocity_, owner_->color_);
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
            amount = timer::frameTime()*0.75f;
            velocity_ += velocity*0.0005f;
            // chance to spawn smoke
            if (sf::Randomizer::Random(0.f, 100.f)/settings::C_globalParticleCount < 0.1f) particles::spawn(particles::pSmoke, location, velocity);
            setDamageSource(with->damageSource());
            break;

        default:;
    }
    life_ -= amount;
}

void Ship::onShockWave(SpaceObject* source, float intensity) {
    setDamageSource(source->damageSource());
    life_ -= intensity;
}

void Ship::setDamageSource(Player* evilOne) {
    damageSource_ = evilOne;
    damageSourceResetTimer_ = 1.f;
}

float Ship::getLife() const {
    return life_ < 0.f ? 0.f : life_;
}

float Ship::getFuel() const {
    return fuel_ < 0.f ? 0.f : fuel_;
}

std::vector<bool> const& Ship::getCollectedItems() const {
    return collectedItems_;
}

void Ship::explode() {
    sound::playSound(sound::ShipExplode, location_, 100.f);
    particles::spawnMultiple(5 , particles::pFragment, location_, location_, location_, owner_->color_);
    particles::spawnMultiple(50, particles::pDust, location_);
    particles::spawnMultiple(20, particles::pExplode, location_);
    particles::spawnMultiple(5, particles::pBurningFragment, location_);
    particles::spawnMultiple(1, particles::pMiniFlame, location_);
    physics::causeShockWave(this, 50.f);
    physics::removeMobileObject(this);
    visible_ = false;
    life_ = 0.f;
    fuel_ = 0.f;
    respawnTimer_ = 10.f;

    ++owner_->deaths_;
    fragStars_ = 0;
    fragStarTimer_ = 0.f;

    if (!damageSource_) damageSource_ = owner_;

    if (damageSource_ == owner_) {
        ++owner_->suicides_;
        announcer::announce(announcer::Affronting);
    }
    else if (damageSource_->team() == owner_->team()) {
        ++damageSource_->teamKills_;
        damageSource_->ship()->fragStars_ = 0;
        damageSource_->ship()->fragStarTimer_ = 0.f;
        announcer::announce(announcer::Affronting);
    }
    else {
        ++damageSource_->frags_;
        ++damageSource_->points_;
        if (games::type() != games::gSpaceBall && games::type() != games::gCannonKeep)
            ++damageSource_->team()->points_;
        ++damageSource_->ship()->fragStars_;
        damageSource_->ship()->fragStarTimer_ = 15.f;
        announcer::announce(announcer::Praising);
    }

}

void Ship::respawn() {
    physics::addMobileObject(this);
    location_ = respawnLocation_;
    rotation_ = respawnRotation_;
    velocity_ = Vector2f();
    rotateSpeed_ = (1.f);
    life_ = 100.f;
    fuel_ = 100.f;
    visible_ = true;
    sound::playSound(sound::ShipRespawn, location_, 100.f);
}

