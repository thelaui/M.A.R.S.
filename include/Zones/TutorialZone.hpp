/* TutorialZone.hpp

Copyright (c) 2010 - 2011 by Felix Lauer and Simon Schneegans

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


# ifndef TUTORIALZONE_HPP_INCLUDED
# define TUTORIALZONE_HPP_INCLUDED

# include "Zones/Zone.hpp"

/// Zone: TutorialZone.
/// A beautiful pinkish Zone, employed in the Tutorial.

class TutorialZone: public Zone {
    public:
        /// Ctor, which creates the Zone.
        TutorialZone(Vector2f const& location, float radius);

        /// Returns true, if the given SpaceObject is inside this Zone.
        bool isInside(SpaceObject const& toBeChecked) const;

        /// Draws the zone.
        void draw() const;

    private:
        const float radius_;
        Vector2f location_;
};

# endif // TUTORIALZONE_HPP_INCLUDED


