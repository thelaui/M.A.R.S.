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

# include "Particles/fuels.hpp"
# include "Particles/muds.hpp"
# include "Particles/smokes.hpp"
# include "Particles/eruptions.hpp"
# include "Particles/ammoAFK47s.hpp"
# include "Particles/ammoROFLEs.hpp"
# include "Particles/dusts.hpp"
# include "Particles/explodes.hpp"
# include "Particles/burningFragments.hpp"
# include "Particles/fragmentFlames.hpp"
# include "Particles/miniFlames.hpp"
# include "Particles/ammoShotguns.hpp"
# include "Particles/ammoFlubbas.hpp"
# include "Particles/miniAmmoFlubbas.hpp"
# include "Particles/cannonBalls.hpp"
# include "Particles/sparks.hpp"
# include "Particles/fragments.hpp"
# include "Particles/ammoBurners.hpp"
# include "Particles/ammoH2OMGs.hpp"
# include "Particles/heats.hpp"
# include "Particles/heatJets.hpp"
# include "Particles/shockWaves.hpp"
# include "Particles/heatBurners.hpp"

namespace particles {
    void update() {
        fuels::             update();
        muds::              update();
        smokes::            update();
        eruptions::         update();
        ammoAFK47s::        update();
        ammoROFLEs::        update();
        ammoShotguns::      update();
        dusts::             update();
        explodes::          update();
        burningFragments::  update();
        fragmentFlames::    update();
        miniFlames::        update();
        ammoFlubbas::       update();
        miniAmmoFlubbas::   update();
        cannonBalls::       update();
        sparks::            update();
        fragments::         update();
        ammoBurners::       update();
        ammoH2OMGs::        update();
        heats::             update();
        heatJets::          update();
        shockWaves::        update();
        heatBurners::       update();
    }

    void drawLower() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUADS);

            explodes::          draw();
            ammoFlubbas::       draw();
            smokes::            draw();
            dusts::             draw();
            miniAmmoFlubbas::   draw();
            miniFlames::        draw();
            ammoH2OMGs::        draw();
            eruptions::         draw();
            muds::              draw();
            ammoShotguns::      draw();
            ammoAFK47s::        draw();
            ammoROFLEs::        draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        fragments::         draw();

    }

    void drawHigher() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUADS);

            fuels::             draw();
            cannonBalls::       draw();
            ammoBurners::       draw();
            fragmentFlames::    draw();
            sparks::            draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void drawHeat() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Particles));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);

            heats::        draw();
            heatJets::     draw();
            heatBurners::  draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::ShockWave));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);

            shockWaves::   draw();

        glEnd();
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void spawn(ParticleType const& type, Vector2f const& location, Vector2f const& direction,
               Vector2f const& velocity, Color3f const& color, Player* damageSource) {
        switch (type) {
            case pFuel:             fuels::             spawn(location, direction, velocity);               break;
            case pMud:              muds::              spawn(location, direction, velocity, color);        break;
            case pSmoke:            smokes::            spawn(location);                                    break;
            case pEruption:         eruptions::         spawn(location, direction, velocity);               break;
            case pAmmoAFK47:        ammoAFK47s::        spawn(location, direction, velocity, damageSource); break;
            case pAmmoROFLE:        ammoROFLEs::        spawn(location, direction, velocity, damageSource); break;
            case pAmmoShotgun:      ammoShotguns::      spawn(location, direction, velocity, damageSource); break;
            case pDust:             dusts::             spawn(location);                                    break;
            case pExplode:          explodes::          spawn(location);                                    break;
            case pBurningFragment:  burningFragments::  spawn(location);                                    break;
            case pFragmentFlame:    fragmentFlames::    spawn(location, direction, velocity);               break;
            case pMiniFlame:        miniFlames::        spawn(location);                                    break;
            case pAmmoFlubba:       ammoFlubbas::       spawn(location, direction, velocity, damageSource); break;
            case pMiniAmmoFlubba:   miniAmmoFlubbas::   spawn(location);                                    break;
            case pCannonBall:       cannonBalls::       spawn(location, direction);                         break;
            case pSpark:            sparks::            spawn(location, direction, velocity, color);        break;
            case pFragment:         fragments::         spawn(location, color);                             break;
            case pAmmoBurner:       ammoBurners::       spawn(location, direction, velocity, damageSource); break;
            case pAmmoH2OMG:        ammoH2OMGs::        spawn(location, direction, velocity, damageSource); break;
            case pHeat:             heats::             spawn(location);                                    break;
            case pHeatJet:          heatJets::          spawn(location, direction, velocity);               break;
            case pShockWave:        shockWaves::        spawn(location);                                    break;
            case pHeatBurner:       heatBurners::       spawn(location, direction, velocity);               break;
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
        smokes::shockWave(source, strength, radius);
        dusts:: shockWave(source, strength, radius);
        ammoFlubbas:: shockWave(source, strength, radius);
    }

    int count() {
        return fuels::count() + muds::count() + smokes::count() + eruptions::count()
                + ammoAFK47s::count() + ammoROFLEs::count() + dusts::count() + explodes::count()
                + burningFragments::count() + fragmentFlames::count() + ammoShotguns::count()
                + miniFlames::count() + ammoFlubbas::count() + miniAmmoFlubbas::count()
                + cannonBalls::count() + sparks::count() + fragments::count() + ammoBurners::count()
                + ammoH2OMGs::count() + heats::count() + heatJets::count() + shockWaves::count()
                + heatBurners::count();
    }

    void clear() {
        fuels::             clear();
        muds::              clear();
        smokes::            clear();
        eruptions::         clear();
        ammoAFK47s::        clear();
        ammoROFLEs::        clear();
        ammoShotguns::      clear();
        dusts::             clear();
        explodes::          clear();
        burningFragments::  clear();
        fragmentFlames::    clear();
        miniFlames::        clear();
        ammoFlubbas::       clear();
        miniAmmoFlubbas::   clear();
        cannonBalls::       clear();
        sparks::            clear();
        fragments::         clear();
        ammoBurners::       clear();
        ammoH2OMGs::        clear();
        heats::             clear();
        heatJets::          clear();
        shockWaves::        clear();
        heatBurners::       clear();
    }
}
