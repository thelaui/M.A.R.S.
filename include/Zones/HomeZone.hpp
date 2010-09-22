# ifndef HOMEZONE_HPP_INCLUDED
# define HOMEZONE_HPP_INCLUDED

# include "Zones/Zone.hpp"

class HomeZone: public Zone {
    public:
        HomeZone(Vector2f const& location);

        bool isInside(SpaceObject const& toBeChecked) const;
        void draw() const;

    private:
        float radius_;
        Vector2f location_;
        int homeSide_;
};

# endif // HOMEZONE_HPP_INCLUDED
