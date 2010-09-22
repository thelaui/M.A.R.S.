/* stars.hpp

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

# ifndef STARS_HPP_INCLUDED
# define STARS_HPP_INCLUDED

# include "System/Vector2f.hpp"
# include "System/Color3f.hpp"

namespace stars {

    class Star {
        public:
            Star();
            void draw() const;

        private:
            Vector2f location_;
            float    radius_;
            Color3f  color_;
    };

    void draw();
    void populateSpace();
}

# endif // STARS_HPP_INCLUDED
