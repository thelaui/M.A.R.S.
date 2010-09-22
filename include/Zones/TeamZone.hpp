# ifndef TEAMZONE_HPP_INCLUDED
# define TEAMZONE_HPP_INCLUDED

# include "Zone.hpp"

class TeamZone: public Zone {
    public:
        TeamZone(int homeSide) :
            Zone(),
            homeSide_(homeSide) {}

        bool isInside(SpaceObject const& toBeChecked) const;
        void draw() const;

    private:
        int homeSide_;
};


# endif // TEAMZONE_HPP_INCLUDED
