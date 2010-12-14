/* Controller.hpp

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

# ifndef CONTROLLER_HPP_INCLUDED
# define CONTROLLER_HPP_INCLUDED

# include "Controllers/controllers.hpp"

class Ship;

/// A base class which controls a Player's Ship.
/// It provides some basic functionallity which directly
/// interacts with the Ship.

class Controller {
    public:
        /// Constructs an Controller, attached to a Player.
        /// \param slave The controlled Player.
        /// \param type The type of the Controller.
        Controller(Player* slave, controllers::ControlType type):
                type_(type),
                slave_(slave) {}

        /// Returns the Ship of the controlled PLayer.
        Ship const* ship() const;

        /// Returns true, if the associated Ship is docked.
        bool shipDocked()  const;

        /// Returns the type of the Controller.
        controllers::ControlType type() const;

    protected:
        /// Accelerates the Ship. (continous action)
        void slaveUp    (bool up)    const;

        /// Turns the Ship left. (continous action)
        void slaveLeft  (bool left)  const;

        /// Turns the Ship right. (continous action)
        void slaveRight (bool right) const;

        /// Fires the Ship's Weapon. (continous action)
        void slaveFire  (bool fire)  const;

        /// "Presses" the Special-key. (single action)
        void slaveSpecial (bool special) const;

        /// "Presses" the Up-key. (single action)
        void slaveUp    () const;

        /// "Presses" the Left-key. (single action)
        void slaveLeft  () const;

        /// "Presses" the Right-key. (single action)
        void slaveRight () const;

        /// "Presses" the Fire-key. (single action)
        void slaveFire  () const;

        /// "Presses" the Special-key. (single action)
        void slaveSpecial () const;

        /// The type of the controller.
        controllers::ControlType type_;

        /// The controlled Player.
        Player* slave_;
};

# endif // CONTROLLER_HPP_INCLUDED
