/* TabStats.hpp

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

# ifndef TABSTATS_HPP_INCLUDED
# define TABSTATS_HPP_INCLUDED

# include "Hud/HudElement.hpp"

# include "Players/Team.hpp"
# include "Players/Player.hpp"

# include <map>
# include <set>

class TabStats: public HudElement {
    public:
        TabStats();

        void update();
        void draw() const;

        void display(bool show = true);
        void refresh();

    private:
        struct playerPtrCmp{
            bool operator()(Player* lhs, Player* rhs){
                return lhs->points_ > rhs->points_;
            }
        };

        struct teamPtrCmp{
            bool operator()(Team* lhs, Team* rhs){
                return lhs->points_ > rhs->points_;
            }
        };

        bool visible_, refresh_;
        int sumPoints_;
        std::multimap<Team*, std::multiset<Player*, playerPtrCmp>, teamPtrCmp > teamMap_;
};

# endif // TABSTATS_HPP_INCLUDED







