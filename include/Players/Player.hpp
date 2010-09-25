/* Player.hpp

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

# ifndef PLAYER_HPP_INCLUDED
# define PLAYER_HPP_INCLUDED

# include "SpaceObjects/Ship.hpp"
# include "Players/players.hpp"
# include "Controllers/Controller.hpp"

class Team;

class Player {
    public:
        Player(std::string const& name, Color3f const& color, int  graphic, controllers::ControlType controlType);
        Player(std::string*       name, Color3f*       color, int* graphic, controllers::ControlType controlType);

        void                resetPoints();

        Team*               team()  const {return team_;}
        Ship*               ship()  const {return ship_;}
        Color3f const&      color() const {return colorRef_ == NULL ? color_ : *colorRef_;}
        std::string const&  name()  const {return nameRef_  == NULL ? name_  : *nameRef_ ;}
        int                 graphic()const {return graphicRef_  == NULL ? graphic_  : *graphicRef_ ;}

        friend class Team;
        friend class Ship;
        friend class Controller;
        friend class TabStats;
std::string  name_;
    private:
        Ship* ship_;

        std::string* nameRef_;
        int points_, frags_, suicides_, deaths_, teamKills_;
        Color3f color_;
        Color3f* colorRef_;
        int graphic_;
        int* graphicRef_;
        Team* team_;
        controllers::ControlType controlType_;
};


# endif // PLAYER_HPP_INCLUDED
