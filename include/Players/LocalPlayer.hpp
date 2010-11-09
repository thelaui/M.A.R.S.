/* LocalPlayer.hpp

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

# ifndef LOCALPLAYER_HPP_INCLUDED
# define LOCALPLAYER_HPP_INCLUDED

# include "Players/Player.hpp"

class Team;

class LocalPlayer: public Player {
    public:
        LocalPlayer(controllers::ControlType type);

        Color3f const&           color()  const {return *color_;}
        sf::String const&        name()   const {return *name_;}
        int                      graphic()const {return *graphic_;}

    private:
        sf::String*  name_;
        Color3f* color_;
        int* graphic_;
};

# endif // LOCALPLAYER_HPP_INCLUDED


