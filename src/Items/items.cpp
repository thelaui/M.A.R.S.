/* items.cpp

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

# include "Items/items.hpp"

# include "Items/CannonControl.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Home.hpp"
# include "Items/PowerUp.hpp"
# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Items/PUShield.hpp"

# include <list>

namespace items {

    namespace {
        CannonControl* cannonControl_(NULL);
        std::list<PowerUp*> powerUps_ = std::list<PowerUp*>();

        void spawnPowerUp() {
            int tries(0);
            bool newPowerUpFits(false);
            Vector2f position(0.f, 0.f);

            while (!newPowerUpFits && ++tries < 500) {
                // 100 is min distance between edge and planet
                int randx = rand() % (1280 - 2*(50)) + 50;
                int randy = rand() % (800  - 2*(50)) + 50 ;
                position.x_ = randx;
                position.y_ = randy;

                // check for collisions with other objects
                newPowerUpFits = true;
                for (std::vector<SpaceObject*>::const_iterator it = spaceObjects::getObjects().begin(); it != spaceObjects::getObjects().end(); ++it)
                    if (((*it)->location() - position).lengthSquare() < std::pow((*it)->radius() + 50, 2))
                        newPowerUpFits = false;

                if (newPowerUpFits) break;
            }
            if(newPowerUpFits)
                switch (sf::Randomizer::Random(static_cast<int>(iShield), static_cast<int>(COUNT))) {
                    case iShield:
                        powerUps_.push_back(new PUShield(position, sf::Randomizer::Random(8.f, 12.f))); break;
                    case iReverse:
                        powerUps_.push_back(new PUShield(position, sf::Randomizer::Random(8.f, 12.f))); break;
                    case iSleep:
                        powerUps_.push_back(new PUShield(position, sf::Randomizer::Random(8.f, 12.f))); break;
                    case iHealth:
                        powerUps_.push_back(new PUShield(position, sf::Randomizer::Random(8.f, 12.f))); break;
                }
        }
    }
    void update() {
        if (cannonControl_)
            cannonControl_->update();

        if (settings::C_powerUpRate > 0) {
            if (sf::Randomizer::Random(0, (101-settings::C_powerUpRate)*15 + 80) < 150.f * timer::frameTime())
                spawnPowerUp();
        }

        std::list<PowerUp*>::iterator it = powerUps_.begin();
        while (it != powerUps_.end()) {
             (*it)->update();
             if ((*it)->isDead()) {
                delete *it;
                it = powerUps_.erase(it);
             }
             else ++it;
        }
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::PowerUps));

        if (cannonControl_)
            cannonControl_->draw();

        for (std::list<PowerUp*>::iterator it = powerUps_.begin(); it != powerUps_.end(); ++it)
            (*it)->draw();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void addCannonControl() {
        // temporary list of all homes
        std::vector<Home*>const& homes = spaceObjects::getHomes();

        if (homes.size() >= 2) {
            Vector2f midPoint;
            for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it)
                midPoint += (*it)->location();
            midPoint /= homes.size();
            cannonControl_ = new CannonControl(midPoint);
        }
        else
            cannonControl_ = new CannonControl(Vector2f());
    }

    CannonControl* getCannonControl() {
        return cannonControl_;
    }

    void removeItem(Item* toRemove) {
        if (toRemove == cannonControl_)
            delete cannonControl_;
        else {
            std::list<PowerUp*>::iterator it = powerUps_.begin();
            while (it != powerUps_.end()) {
                 if (*it == toRemove) {
                    delete *it;
                    it = powerUps_.erase(it);
                 }
                 else ++it;
            }
        }
    }

    void clear() {
        if (cannonControl_) {
            delete cannonControl_;
            cannonControl_ = NULL;
        }
        for (std::list<PowerUp*>::iterator it = powerUps_.begin(); it != powerUps_.end(); ++it)
            delete(*it);
        powerUps_.clear();
    }
}
