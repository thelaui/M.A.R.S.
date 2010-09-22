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
    }

    void drawLower() {
        muds::              draw();
        smokes::            draw();
        eruptions::         draw();
        ammoAFK47s::        draw();
        ammoROFLEs::        draw();
        ammoShotguns::      draw();
        dusts::             draw();
        fragmentFlames::    draw();
        ammoFlubbas::       draw();
        miniAmmoFlubbas::   draw();
        miniFlames::        draw();
        fragments::         draw();
        explodes::          draw();
    }

    void drawHigher() {
        fuels::             draw();
        cannonBalls::       draw();
        sparks::            draw();
    }

    void spawn(ParticleType const& type, Vector2f const& location, Vector2f const& direction,
               Vector2f const& velocity, Color3f const& color, Player* damageSource) {
        switch (type) {
            case pFuel:             fuels::             spawn(location, direction, velocity);               break;
            case pMud:              muds::              spawn(location, direction, velocity, color);        break;
            case pSmoke:            smokes::            spawn(location, direction);                         break;
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
                + cannonBalls::count() + sparks::count() + fragments::count();
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
    }
}
