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
# include "Items/items.hpp"

class Ship;

class Item {
    public:
        Item(items::ItemType type, Vector2f const& location, float radius, Ship* ship, int texX, int texY):
            location_(location),
            radius_(radius),
            ship_(ship),
            collected_(false),
            texX_(texX),
            texY_(texY),
            type_(type) {}

        ~Item();

        virtual void update();
        virtual void draw() const;

        Vector2f const& location() const;
        float           radius()   const;

    protected:
        Vector2f location_;
        float radius_;
        Ship* ship_;
        bool collected_;
        items::ItemType type_;

    private:
        int texX_, texY_;
};

# endif // ITEM_HPP_INCLUDED





