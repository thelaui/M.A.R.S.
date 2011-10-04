/* Ship.hpp

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

# ifndef SHIP_HPP_INCLUDED
# define SHIP_HPP_INCLUDED

# include "SpaceObjects/MobileSpaceObject.hpp"
# include "Weapons/Weapon.hpp"
# include "Specials/Special.hpp"
# include "DecoObjects/Ice.hpp"

# define ITEM_CANNON_CONTROL 0

class Player;
class PowerUp;

class Ship: public MobileSpaceObject {
    public:
        Ship(Vector2f const& location, float rotation, Player* owner);

        void update();
        void draw() const;
        void drawWeapon() const;

        void onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity);

        void onShockWave(Player* damageSource, float intensity);

        void setDamageSource(Player* evilOne);
        void drainLife(Player* source, int amount, Vector2f const& direction, float waitForOtherDamage = 0.001f);

        void heal(Player* source, int amount);
        void refuel(Player* source, int amount);

        float   getLife()  const;
        float   getFuel()  const;
        Player* getOwner() const;

        float   rotation() const;

        bool    collidable() const;
        bool    attackable() const;

        std::vector<PowerUp*> const& getCollectedPowerUps() const;

        friend class Controller;
        friend class BotController;
        friend class KeyController;
        friend class CannonControl;
        friend class PowerUp;
        friend class PUHealth;
        friend class PUFuel;
        friend class ShipName;
        friend class ShipHighlight;
        friend class Tutorial;
        friend class Blast;
        friend class Freezer;
        friend class FireWall;
        friend class Shocker;
        friend class Heal;
        friend class Special;
        friend class Weapon;
        template <typename Object> friend class Ice;
        friend class Team;

    private:

        void explode();
        void respawn();

        Player* owner_;

        float rotation_;
        float rotateSpeed_;
        int up_, left_, right_;
        bool docked_;
        bool weaponChange_;
        bool specialChange_;

        bool visible_;
        float ghostTimer_;
        float frozen_;
        float respawnTimer_;
        float damageSourceResetTimer_;

        Vector2f respawnLocation_;
        float respawnRotation_;

        Weapon* currentWeapon_;
        Special* currentSpecial_;

        float life_, maxLife_;
        float fuel_, maxFuel_;

        std::vector<PowerUp*> collectedPowerUps_;

        int fragStars_;
        float damageByLocalPlayer_;
        float damageCheckTimer_;
        Vector2f damageDirection_;
        int collisionCount_;
};

# endif // SHIP_HPP_INCLUDED
