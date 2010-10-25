/* PowerUp.hpp

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

# ifndef POWERUP_HPP_INCLUDED
# define POWERUP_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "Items/Item.hpp"
# include "Items/items.hpp"

class Ship;

class PowerUp: public Item{
    public:
        PowerUp(items::ItemType type, Vector2f const& location, float lifeTime):
            Item(location, 15.f, NULL),
            type_(type),
            lifeTime_(lifeTime){}

        ~PowerUp();

        void update();

        bool isDead()             const { return lifeTime_ <= 0.f; }

        float radius()            const { return radius_;     }
        bool collected()          const { return collected_ ; }
        Vector2f const location() const { return location_;   }
        Ship* ship()              const { return ship_;       }

    protected:
        items::ItemType type_;
        float lifeTime_;
};

# endif // POWERUP_HPP_INCLUDED






