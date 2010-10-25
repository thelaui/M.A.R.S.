/* particles.cpp

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

# include "Particles/particles.hpp"

# include "System/settings.hpp"
# include "System/timer.hpp"

# include "Particles/AmmoAFK47.hpp"
# include "Particles/AmmoBurner.hpp"
# include "Particles/AmmoFlubba.hpp"
# include "Particles/Fuel.hpp"
# include "Particles/Mud.hpp"
# include "Particles/Smoke.hpp"
# include "Particles/Eruption.hpp"
# include "Particles/AmmoROFLE.hpp"
# include "Particles/Dust.hpp"
# include "Particles/Explode.hpp"
# include "Particles/BurningFragment.hpp"
# include "Particles/FragmentFlame.hpp"
# include "Particles/MiniFlame.hpp"
# include "Particles/AmmoShotgun.hpp"
# include "Particles/MiniAmmoFlubba.hpp"
# include "Particles/CannonBall.hpp"
# include "Particles/Spark.hpp"
# include "Particles/Fragment.hpp"
# include "Particles/AmmoH2OMG.hpp"
# include "Particles/Heat.hpp"
# include "Particles/HeatJet.hpp"
# include "Particles/ShockWave.hpp"
# include "Particles/HeatBurner.hpp"

namespace particles {
    void update() {
        AmmoAFK47::        updateAll();
        AmmoBurner::       updateAll();
        AmmoFlubba::       updateAll();
        Fuel::             updateAll();
        Mud::              updateAll();
        Smoke::            updateAll();
        Eruption::         updateAll();
        AmmoROFLE::        updateAll();
        AmmoShotgun::      updateAll();
        Dust::             updateAll();
        Explode::          updateAll();
        BurningFragment::  updateAll();
        FragmentFlame::    updateAll();
        MiniFlame::        updateAll();
        MiniAmmoFlubba::   updateAll();
        CannonBall::       updateAll();
        Spark::            updateAll();
        Fragment::         updateAll();
        AmmoH2OMG::        updateAll();
        Heat::             updateAll();
        HeatJet::          updateAll();
        ShockWave::        updateAll();
        HeatBurner::       updateAll();
    }

    void drawLower() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUADS);

            AmmoAFK47::        drawAll();
            AmmoFlubba::       drawAll();
            Explode::          drawAll();
            Smoke::            drawAll();
            Dust::             drawAll();
            MiniAmmoFlubba::   drawAll();
            MiniFlame::        drawAll();
            AmmoH2OMG::        drawAll();
            Eruption::         drawAll();
            Mud::              drawAll();
            AmmoShotgun::      drawAll();
            AmmoROFLE::        drawAll();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        Fragment::         drawAll();

    }

    void drawHigher() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUADS);

            AmmoBurner::       drawAll();
            Fuel::             drawAll();
            CannonBall::       drawAll();
            FragmentFlame::    drawAll();
            Spark::            drawAll();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void drawHeat() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);

            Heat::        drawAll();
            HeatJet::     drawAll();
            HeatBurner::  drawAll();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShockWave));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);

            ShockWave::   drawAll();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void spawn(ParticleType const& type, Vector2f const& location, Vector2f const& direction,
               Vector2f const& velocity, Color3f const& color, Player* damageSource) {
        switch (type) {
            case pAmmoAFK47:        AmmoAFK47::        spawn(location, direction, velocity, color, damageSource); break;
            case pAmmoBurner:       AmmoBurner::       spawn(location, direction, velocity, color, damageSource); break;
            case pAmmoFlubba:       AmmoFlubba::       spawn(location, direction, velocity, color, damageSource); break;
            case pFuel:             Fuel::             spawn(location, direction, velocity, color, damageSource); break;
            case pMud:              Mud::              spawn(location, direction, velocity, color, damageSource); break;
            case pSmoke:            Smoke::            spawn(location, direction, velocity, color, damageSource); break;
            case pEruption:         Eruption::         spawn(location, direction, velocity, color, damageSource); break;
            case pAmmoROFLE:        AmmoROFLE::        spawn(location, direction, velocity, color, damageSource); break;
            case pAmmoShotgun:      AmmoShotgun::      spawn(location, direction, velocity, color, damageSource); break;
            case pDust:             Dust::             spawn(location, direction, velocity, color, damageSource); break;
            case pExplode:          Explode::          spawn(location, direction, velocity, color, damageSource); break;
            case pBurningFragment:  BurningFragment::  spawn(location, direction, velocity, color, damageSource); break;
            case pFragmentFlame:    FragmentFlame::    spawn(location, direction, velocity, color, damageSource); break;
            case pMiniFlame:        MiniFlame::        spawn(location, direction, velocity, color, damageSource); break;
            case pMiniAmmoFlubba:   MiniAmmoFlubba::   spawn(location, direction, velocity, color, damageSource); break;
            case pCannonBall:       CannonBall::       spawn(location, direction, velocity, color, damageSource); break;
            case pSpark:            Spark::            spawn(location, direction, velocity, color, damageSource); break;
            case pFragment:         Fragment::         spawn(location, direction, velocity, color, damageSource); break;
            case pAmmoH2OMG:        AmmoH2OMG::        spawn(location, direction, velocity, color, damageSource); break;
            case pHeat:             Heat::             spawn(location, direction, velocity, color, damageSource); break;
            case pHeatJet:          HeatJet::          spawn(location, direction, velocity, color, damageSource); break;
            case pShockWave:        ShockWave::        spawn(location, direction, velocity, color, damageSource); break;
            case pHeatBurner:       HeatBurner::       spawn(location, direction, velocity, color, damageSource); break;
        }
    }

    void spawnMultiple(float amount, ParticleType const& type, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource) {
        for (int i = 0; i <= (amount * settings::C_globalParticleCount)/10; ++i)
            spawn(type, location, direction, velocity, color, damageSource);
    }

    void spawnTimed(float amount, ParticleType const& type, Vector2f const& location,
                       Vector2f const& direction, Vector2f const& velocity, Color3f const& color, Player* damageSource) {
        for (int i = 0; i <= (amount * settings::C_globalParticleCount)*timer::frameTime(); ++i)
            spawn(type, location, direction, velocity, color, damageSource);
    }

    void shockWave(SpaceObject* source, float strength, float radius) {
        Smoke::shockWave(source, strength, radius);
        Dust:: shockWave(source, strength, radius);
        AmmoFlubba:: shockWave(source, strength, radius);
    }

    int count() {
        return Fuel::count() + Mud::count() + Smoke::count() + Eruption::count()
                + AmmoAFK47::count() + AmmoROFLE::count() + Dust::count() + Explode::count()
                + BurningFragment::count() + FragmentFlame::count() + AmmoShotgun::count()
                + MiniFlame::count() + AmmoFlubba::count() + MiniAmmoFlubba::count()
                + CannonBall::count() + Spark::count() + Fragment::count() + AmmoBurner::count()
                + AmmoH2OMG::count() + Heat::count() + HeatJet::count() + ShockWave::count()
                + HeatBurner::count();
    }

    void clear() {
        AmmoAFK47::        clear();
        AmmoBurner::       clear();
        AmmoFlubba::       clear();
        Fuel::             clear();
        Mud::              clear();
        Smoke::            clear();
        Eruption::         clear();
        AmmoROFLE::        clear();
        AmmoShotgun::      clear();
        Dust::             clear();
        Explode::          clear();
        BurningFragment::  clear();
        FragmentFlame::    clear();
        MiniFlame::        clear();
        MiniAmmoFlubba::   clear();
        CannonBall::       clear();
        Spark::            clear();
        Fragment::         clear();
        AmmoH2OMG::        clear();
        Heat::             clear();
        HeatJet::          clear();
        ShockWave::        clear();
        HeatBurner::       clear();
    }
}
