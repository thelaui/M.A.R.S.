/* Home.cpp

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

# include "SpaceObjects/Home.hpp"

# include "Particles/particles.hpp"
# include "Players/players.hpp"
# include "SpaceObjects/ships.hpp"
# include "Media/sound.hpp"
# include "Media/announcer.hpp"
# include "Hud/hud.hpp"
# include "System/window.hpp"
# include "Games/games.hpp"
# include "Players/Team.hpp"
# include "SpaceObjects/Ball.hpp"

# include <sstream>

Home::Home(Vector2f const& location, float radius, Color3f const& color):
               SpaceObject(spaceObjects::oHome, location, radius, radius*150.f),
               color_(color),
               life_(1000.f),
               visible_(true),
               restartTimer_(0.f) {
    physics::addStaticObject(this);
    physics::addGravitySource(this);
}

void Home::update() {
    if (restartTimer_ > 0) {
        restartTimer_ -= timer::frameTime();
        if (restartTimer_ <= 0) games::restart();
    }
}

void Home::draw() const {
    if (visible_) {
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Home));

        color_.gl3f();
        float drawRadius = radius_*1.7f;
        glBegin(GL_QUADS);
            glTexCoord2i(0, 0); glVertex2f(location_.x_-drawRadius, location_.y_-drawRadius);
            glTexCoord2i(0, 1); glVertex2f(location_.x_-drawRadius, location_.y_+drawRadius);
            glTexCoord2i(1, 1); glVertex2f(location_.x_+drawRadius, location_.y_+drawRadius);
            glTexCoord2i(1, 0); glVertex2f(location_.x_+drawRadius, location_.y_-drawRadius);
        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Home::drawLife() const {
    if (visible_) {
        std::stringstream sstr;
        sstr << static_cast<int>(ceil(life_/10.f));
        hud::drawSpaceText(sstr.str(), location_ + Vector2f(0.f, -20), font::HandelGotDLig, 20.f, TEXT_ALIGN_LEFT, Color3f(0.6, 0.6, 0.6));
    }
}

void Home::createShips(std::vector<Player*>& inhabitants) const {
    // set ship locations....
    // calculate available surface angle of homePlanet, looking at two cases:
    if (location_.x_ < radius_) {
        // 1. homeplanet is covered by left screen edge
        float angle = std::acos(-location_.x_/radius_);
        float deltaAngle = 2*angle/(inhabitants.size()+1);
        angle = ((inhabitants.size()+1)%2)*deltaAngle/2;
        int shipCounter = 0;

         for (std::vector<Player*>::iterator it = inhabitants.begin(); it != inhabitants.end(); ++it) {
            // calc location of ship
            angle += deltaAngle*shipCounter*std::pow(-1.0, shipCounter);
            Vector2f location = Vector2f(std::cos(angle), std::sin(angle)) * (radius_+10)+location_;
            float    rotation = angle*180/M_PI;
            ships::addShip(location, rotation, *it);
            ++shipCounter;
        }
    }
    else if (location_.x_ > 1280 - radius_) {
        // 2. homeplanet is covered by right screen edge
        float angle = std::acos((location_.x_ - 1280)/radius_);
        float deltaAngle = 2*angle/(inhabitants.size()+1);
        angle = ((inhabitants.size()+1)%2)*deltaAngle/2;
        int shipCounter = 0;

        for (std::vector<Player*>::iterator it = inhabitants.begin(); it != inhabitants.end(); ++it) {
            // calc location of ship
            angle += deltaAngle*shipCounter*std::pow(-1.0, shipCounter);
            Vector2f location = Vector2f(-std::cos(angle), std::sin(angle)) * (radius_+10)+location_;
            float    rotation = 180-angle*180/M_PI;
            ships::addShip(location, rotation, *it);
            ++shipCounter;
        }
    }
    else {
        // 3. homeplanet is entirely visible
        float deltaAngle = 2*M_PI/(inhabitants.size());
        float angle = 0;

        for (std::vector<Player*>::iterator it = inhabitants.begin(); it != inhabitants.end(); ++it) {
            // calc location of ship
            angle += deltaAngle;
            Vector2f location = Vector2f(std::cos(angle), std::sin(angle)) * (radius_+10)+location_;
            float    rotation = angle*180/M_PI;
            ships::addShip(location, rotation, *it);
        }
    }
}

void Home::onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();

    switch (with->type()) {
        case spaceObjects::oAmmoROFLE:
            particles::spawnMultiple(20, particles::pMud, location, direction, velocity, color_);
            break;

        case spaceObjects::oAmmoShotgun:
            particles::spawnMultiple(2, particles::pMud, location, direction, velocity, color_);
            break;

        case spaceObjects::oAmmoAFK47:
            particles::spawnMultiple(2, particles::pMud, location, direction, velocity, color_);
            break;

        case spaceObjects::oShip:
            if (strength > 50)
                particles::spawnMultiple(10, particles::pMud, location, direction, velocity, color_);
            break;

        case spaceObjects::oBall:
            life_ -= (strength + dynamic_cast<Ball*>(with)->heatAmount());
            if (life_ <= 0) explode();
            break;

        case spaceObjects::oCannonBall:
            life_ -= sf::Randomizer::Random(40, 60);
            if (life_ <= 0) explode();
            break;

        case spaceObjects::oAmmoBurner:
            particles::spawn(particles::pMiniFlame, location);
            break;

        default:;
    }
}

void Home::explode() {
    sound::playSound(sound::PlanetExplode, location_, 100.f);
    announcer::announce(announcer::Neutral);
    physics::causeShockWave(this, 100.f);
    particles::spawnMultiple(200, particles::pDust, location_);
    particles::spawnMultiple(20, particles::pExplode, location_);
    particles::spawnMultiple(2, particles::pBurningFragment, location_);
    location_ = Vector2f(50000.f, 50000.f);
    visible_ = false;
    restartTimer_ = 5.f;
    players::getTeamL()->home() == this ? ++players::getTeamR()->points_ : ++players::getTeamL()->points_;
    hud::displayPoints();
}

