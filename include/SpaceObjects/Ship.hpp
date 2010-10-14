/* Ship.hpp

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

# ifndef SHIP_HPP_INCLUDED
# define SHIP_HPP_INCLUDED

# include "SpaceObjects/MobileSpaceObject.hpp"
# include "Weapons/Weapon.hpp"

# define ITEM_CANNON_CONTROL 0

class Player;

class Ship: public MobileSpaceObject {
    public:
        Ship(Vector2f const& location, float rotation, Player* owner);

        void update();
        void draw() const;
        void drawName() const;
        void drawHighLight() const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

        void onShockWave(SpaceObject* source, float intensity);

        void setDamageSource(Player* evilOne);

        float  getLife() const;
        float  getFuel() const;

        std::vector<bool> const& getCollectedItems() const;

        friend class Controller;
        friend class BotController;
        friend class AFK47;
        friend class ROFLE;
        friend class Shotgun;
        friend class Flubba;
        friend class Burner;
        friend class H2OMG;
        friend class CannonControl;
        friend class ShipName;
        friend class ShipHighlight;
        friend class Tutorial;
        friend class TutBot;
        friend class TutAggroBot;

    private:

        void explode();
        void respawn();

        Player* owner_;

        float rotation_;
        float rotateSpeed_;
        bool up_, left_, right_;
        bool docked_;
        bool weaponChange_;

        bool visible_;
        float respawnTimer_;
        float damageSourceResetTimer_;

        Vector2f respawnLocation_;
        float respawnRotation_;

        Weapon* currentWeapon_;

        float life_;
        float fuel_;

        std::vector<bool> collectedItems_;

        int fragStars_, rememberedReputation_;
        float fragStarTimer_, pointCheckTimer_;
};

# endif // SHIP_HPP_INCLUDED
