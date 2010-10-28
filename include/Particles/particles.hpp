/* particles.hpp

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

# ifndef PARTICLES_HPP_INCLUDED
# define PARTICLES_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

class Player;
class SpaceObject;

/// A namespace which handles everything related to particles.
/// The functions in this namespace can spawn, draw, count and
/// clear all particles.

namespace particles {
    /// A list of all supported particle types.
    enum ParticleType {
                    pFuel,              ///< Spawned by the Ships exhaust.
                    pSpark,             ///< Spawned when a Particle collides with a Ship, or a Ship with another Ship.
                    pDust,              ///< Spawned by explosions - they form the ring.
                    pAmmoFlubba,        ///< Spawned by the Flubba.
                    pExplode,           ///< Spawned by explosions (the yellowish glow).
                    pAmmoAFK47,         ///< Spawned by the AFK47.
                    pFragment,          ///< Spawned by explosions.
                    pAmmoShotgun,       ///< Spawned by the Shotgun.
                    pAmmoROFLE,         ///< Spawned by the ROFLE.
                    pMiniAmmoFlubba,    ///< Spawned by the pAmmoFlubba.
                    pMud,               ///< Spawned by particles colliding with planets.
                    pAmmoBurner,        ///< Spawned by the Burner.
                    pMiniFlame,         ///< Spawned by a BurningFragment when it hits a Planet.
                    pSmoke,             ///< Spawned by a BurningFragment.
                    pEruption,          ///< Spawned by a Sun.
                    pFragmentFlame,     ///< Spawned by a BurningFragment
                    pBurningFragment,   ///< Spawned by explosions.
                    pCannonBall,        ///< Spawned by the Cannon of CannonKeep.
                    pAmmoH2OMG,         ///< Spawned by the H2OMG.
                    pHeat,              ///< Spawned by a MiniFlame.
                    pHeatJet,           ///< Spawned by the Ships exhaust
                    pShockWave,         ///< Spawned by shock waves.
                    pHeatBurner,        ///< Spawned by the Burner (for bumpmapping).
                    pMiniFlameSmoke     ///< Spawned by MiniFlames.
    };

    /// Updates all particles.
    void update();

    /// Draws some particles.
    void drawLower();

    /// Draws some particles.
    void drawHigher();

    /// Draws postFX particles.
    /// Should be called, when a bumpmap is the active rendering content.
    void drawHeat();

    /// Spawns a new Particle.
    /// All the following parameters can be set, but some will be ignored, depending on the ParticleType.
    /// See the individual ctors of the particles for information on which parameters are neccessary!
    /// \param type         The ParticleType of the particle to be spawned.
    /// \param location     The location of the source.
    /// \param direction    The direction in which the particle should be spawned.
    /// \param velocity     The velocity of the source.
    /// \param color        The color of the particle to be spawned.
    /// \param damageSource The player who spawned the particle.
    void spawn(ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);

    /// Spawns multiple Particles.
    /// All the following parameters can be set, but some will be ignored, depending on the ParticleType.
    /// See the individual ctors of the particles for information on which parameters are neccessary!
    /// \param amount       The amount of particles to be spawned. Scales with settings::C_globalParticleCount.
    /// \param type         The ParticleType of the particle to be spawned.
    /// \param location     The location of the source.
    /// \param direction    The direction in which the particle should be spawned.
    /// \param velocity     The velocity of the source.
    /// \param color        The color of the particle to be spawned.
    /// \param damageSource The player who spawned the particle.
    void spawnMultiple(float amount, ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);

    /// Spawns multiple Particles, based on frame time (if a frame took longer, more particles will be spawned).
    /// All the following parameters can be set, but some will be ignored, depending on the ParticleType.
    /// See the individual ctors of the particles for information on which parameters are neccessary!
    /// \param amount       The amount of particles to be spawned. Scales with settings::C_globalParticleCount and the frame time.
    /// \param type         The ParticleType of the particle to be spawned.
    /// \param location     The location of the source.
    /// \param direction    The direction in which the particle should be spawned.
    /// \param velocity     The velocity of the source.
    /// \param color        The color of the particle to be spawned.
    /// \param damageSource The player who spawned the particle.
    void spawnTimed   (float amount, ParticleType const& type, Vector2f const& location, Vector2f const& direction = Vector2f(),
               Vector2f const& velocity = Vector2f(), Color3f const& color = Color3f(1.0f, 1.0f, 1.0f), Player* damageSource = NULL);

    /// Particles, which are affected by shock waves are noticed with this function, that there are any.
    void shockWave(SpaceObject* source, float strength, float radius);

    /// Returns the total amount of particles.
    int  count();

    /// Removes all particles.
    void clear();
}

# endif // PARTICLES_HPP_INCLUDED
