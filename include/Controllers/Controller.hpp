/* Controller.hpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# ifndef CONTROLLER_HPP_INCLUDED
# define CONTROLLER_HPP_INCLUDED

# include "Controllers/controllers.hpp"

class Ship;

class Controller {
    public:
        Controller(Player* slave, controllers::ControlType type):
                type_(type),
                slave_(slave) {}

        Ship const* ship() const;
        bool shipDocked()  const;
        controllers::ControlType type() const;

    protected:
        void slaveUp    (bool up)    const;
        void slaveLeft  (bool left)  const;
        void slaveRight (bool right) const;
        void slaveFire  (bool fire)  const;

        void slaveUp    () const;
        void slaveLeft  () const;
        void slaveRight () const;
        void slaveFire  () const;

        controllers::ControlType type_;

        Player* slave_;
};

# endif // CONTROLLER_HPP_INCLUDED
