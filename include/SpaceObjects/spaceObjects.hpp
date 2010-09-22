/* spaceObjects.hpp

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

# ifndef SPACEOBJECTS_HPP_INCLUDED
# define SPACEOBJECTS_HPP_INCLUDED

# define HOME_LEFT   0
# define HOME_RIGHT  1
# define HOME_MIDDLE 2

# include "System/Color3f.hpp"
# include "System/Vector2f.hpp"

class Home;
class SpaceObject;

namespace spaceObjects {
    enum ObjectType {oPlanet, oSun, oBlackHole, oHome, oShip, oBall, oFuel, oAmmoAFK47, oAmmoROFLE, oAmmoShotgun, oMud, oSmoke,
                     oEruption, oDust, oExplode, oFragment, oBurningFragment, oFragmentFlame, oMiniFlame, oAmmoFlubba, oMiniAmmoFlubba,
                     oCannonBall, oSpark};

    void update();
    void draw();

    void  addPlanet();
    void  addSun();
    void  addBlackHole();
    Home* addHome(int where, Color3f const& color);

    SpaceObject const* getObstacle(Vector2f const& start, Vector2f const& end, bool avoidBall = false, float minDistance = 0.f);
    bool               isOnLine(Vector2f source, Vector2f direction, Vector2f target, float radius);
    std::vector<Home*>const&                getHomes();
    std::vector<SpaceObject*> const&        getObjects();

    void clear();
}

# endif // SPACEOBJECTS_HPP_INCLUDED

