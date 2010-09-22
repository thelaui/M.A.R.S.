# ifndef TACTICALZONE_HPP_INCLUDED
# define TACTICALZONE_HPP_INCLUDED

# include "Zones/Zone.hpp"

class TacticalZone: public Zone {
    public:
        TacticalZone(Vector2f const& location, float radius);

        bool isInside(SpaceObject const& toBeChecked) const;
        void update();
        void draw() const;

        Vector2f getRandomPoint() const;
        Vector2f const& location() const;
        bool covered() const;

        friend class BotController;

    private:
        float radius_;
        Vector2f location_;
        short homeSide_;
        bool covered_;
        float  shipCount_;
};

# endif // TACTICALZONE_HPP_INCLUDED

