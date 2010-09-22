# ifndef PHYSICS_HPP_INCLUDED
# define PHYSICS_HPP_INCLUDED

# include "System/Vector2f.hpp"

# define MOBILES 1
# define STATICS 2

class SpaceObject;
class MobileSpaceObject;

namespace physics {
    void     collide (MobileSpaceObject* source, int with);
    Vector2f attract (MobileSpaceObject* attracted);
    void     causeShockWave(SpaceObject* source, float strength, float radius = 300.f);

    void addMobileObject (MobileSpaceObject* source);
    void removeMobileObject (MobileSpaceObject* source);
    void addStaticObject (SpaceObject* source);
    void addGravitySource(SpaceObject* source);
    void clear();
}

# endif // PHYSICS_HPP_INCLUDED
