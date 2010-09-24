/* particles.hpp

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

# ifndef PARTICLES_HPP_INCLUDED
# define PARTICLES_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

class Player;
class SpaceObject;

namespace particles {
    enum ParticleType {pFuel, pSpark, pDust, pAmmoFlubba, pExplode, pAmmoAFK47, pFragment,
                       pAmmoShotgun, pAmmoROFLE, pMiniAmmoFlubba, pMud, pAmmoBurner, pMiniFlame, pSmoke,
                       pEruption, pFragmentFlame, pBurningFragment, pCannonBall};

    void update();
    void drawLower();
    void drawHigher();

    void spawn(ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);
    void spawnMultiple(float amount, ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);
    void spawnTimed   (float amount, ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);

    void shockWave(SpaceObject* source, float strength, float radius);

    int  count();
    void clear();
}

# endif // PARTICLES_HPP_INCLUDED
