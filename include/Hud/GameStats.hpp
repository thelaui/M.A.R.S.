/* GameStats.hpp

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

# ifndef GAMESTATS_HPP_INCLUDED
# define GAMESTATS_HPP_INCLUDED

# include "Hud/HudElement.hpp"

class GameStats: public HudElement {
    public:
        GameStats():
            updateTimer_(0.5f),
            particleCount_(0) {}

        void draw() const;

    private:
        mutable float updateTimer_;
        mutable int   particleCount_;
};

# endif // GAMESTATS_HPP_INCLUDED






