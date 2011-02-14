/* items.cpp

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

# include "Items/items.hpp"

# include "Items/CannonControl.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/Home.hpp"
# include "Items/PowerUp.hpp"
# include "System/settings.hpp"
# include "System/timer.hpp"
# include "Items/PUShield.hpp"
# include "Items/PUHealth.hpp"
# include "Items/PUFuel.hpp"
# include "Items/PUSleep.hpp"
# include "Items/PUReverse.hpp"

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
            if(newPowerUpFits) {
                if (sf::Randomizer::Random(0, 4) == 0) {
                    // rare powerUps
                    switch (sf::Randomizer::Random(0, 1)) {
                        case 0:
                            powerUps_.push_back(new PUReverse(position)); break;
                        case 1:
                            powerUps_.push_back(new PUSleep(position)); break;
                    }
                }
                else {
                    // common powerUps
                    switch (sf::Randomizer::Random(0, 2)) {
                        case 0:
                            powerUps_.push_back(new PUShield(position)); break;
                        case 1:
                            powerUps_.push_back(new PUHealth(position)); break;
                        case 2:
                            powerUps_.push_back(new PUFuel(position)); break;
                    }
                }
            }
        }
    }

    void update() {
        if (cannonControl_)
            cannonControl_->update();

        if (settings::C_powerUpRate > 0) {
            static float spawnTimer(1.f);
            spawnTimer -= timer::frameTime();
            if (spawnTimer <= 0.f) {
                spawnPowerUp();
                spawnTimer = sf::Randomizer::Random(0.5f, 1.5f) * ((101-settings::C_powerUpRate) * 0.23f + 1.5f);
            }
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

    std::list<PowerUp*>const& getPowerUps() {
        return powerUps_;
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
