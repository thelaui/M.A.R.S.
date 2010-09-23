/* ships.cpp

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

# include "SpaceObjects/ships.hpp"

# include "SpaceObjects/Ship.hpp"

# include <vector>

namespace ships {
    namespace {
        std::vector<Ship*> shipList_;
    }

    void addShip(Vector2f const& location, float rotation, Player* owner) {
        shipList_.push_back(new Ship(location, rotation, owner));
    }

    void update() {
        for (std::vector<Ship*>::iterator it = shipList_.begin(); it != shipList_.end(); ++it)
            (*it)->update();
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ships));

        for (std::vector<Ship*>::iterator it = shipList_.begin(); it != shipList_.end(); ++it)
            (*it)->draw();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void drawNames() {
        for (std::vector<Ship*>::iterator it = shipList_.begin(); it != shipList_.end(); ++it)
            (*it)->drawName();
    }

    void drawHighLights() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Ships));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (std::vector<Ship*>::iterator it = shipList_.begin(); it != shipList_.end(); ++it)
            (*it)->drawHighLight();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::vector<Ship*> const& getShips() {
        return shipList_;
    }

    void clear() {
        for (std::vector<Ship*>::iterator it = shipList_.begin(); it != shipList_.end(); ++it)
            delete *it;
        shipList_.clear();
    }
}
