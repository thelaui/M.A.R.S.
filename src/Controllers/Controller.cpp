/* Controller.cpp

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

# include "Controllers/Controller.hpp"

# include "Controllers/KeyController.hpp"
# include "Controllers/BotController.hpp"
# include "Players/Player.hpp"

void Controller::slaveUp (bool up) const {
    if (slave_->ship_->visible_)
        slave_->ship_->up_ = up;
}

void Controller::slaveLeft (bool left) const {
    if (!slave_->ship_->weaponChange_ && slave_->ship_->visible_) {
        if (left && slave_->ship_->right_)
            slave_->ship_->right_ = false;
        else
            slave_->ship_->left_ = left;
    }
}

void Controller::slaveRight (bool right) const {
    if (!slave_->ship_->weaponChange_ && slave_->ship_->visible_) {
        if (right && slave_->ship_->left_)
            slave_->ship_->left_ = false;
        else
            slave_->ship_->right_ = right;
    }
}

void Controller::slaveFire (bool fire) const {
    if (fire && !slave_->ship_->docked_ && slave_->ship_->visible_)
        slave_->ship_->currentWeapon_->fire();
}

// single key presses
void Controller::slaveUp () const {

}

void Controller::slaveLeft () const {
    if (slave_->ship_->weaponChange_ && slave_->ship_->visible_)
        slave_->ship_->currentWeapon_->previous();
}

void Controller::slaveRight () const {
    if (slave_->ship_->weaponChange_ && slave_->ship_->visible_)
        slave_->ship_->currentWeapon_->next();
}

void Controller::slaveFire () const {
    if (slave_->ship_->docked_ && slave_->ship_->visible_)
        slave_->ship_->weaponChange_ = !slave_->ship_->weaponChange_;
}

Ship const* Controller::ship() const {
    return slave_->ship_;
}

bool Controller::shipDocked() const {
    return ship()->docked_;
}

controllers::ControlType Controller::type() const {
    return type_;
}
