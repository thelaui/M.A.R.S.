/* Item.hpp

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

# ifndef ITEM_HPP_INCLUDED
# define ITEM_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"
# include "Items/items.hpp"

class Ship;

class PowerUp {
    public:
        PowerUp(items::PowerUpType type, Vector2f const& location, float radius,
                float totalLifeTime, int texX, int texY, Color3f const& bgColor);

        ~PowerUp();

        virtual void update();
        virtual void draw() const;

        Vector2f const& location() const;
        float           radius()   const;

        bool isDead()              const;

    protected:
        virtual void refreshLifeTime() = 0;

        Vector2f location_;
        float radius_;
        Ship* ship_;
        bool collected_;
        items::PowerUpType type_;
        float lifeTime_, totalLifeTime_;

    private:
        int texX_, texY_;
        Color3f bgColor_;
};

# endif // ITEM_HPP_INCLUDED





