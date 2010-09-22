/* stars.cpp

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

# include "SpaceObjects/stars.hpp"

# include "Media/texture.hpp"
# include "System/settings.hpp"

# include <SFML/System.hpp>

namespace stars {
    Star::Star():
        location_(sf::Randomizer::Random(0, 1280), sf::Randomizer::Random(0, 900)),
        radius_(sf::Randomizer::Random(1.0f, 2.5f)),
        color_(sf::Randomizer::Random(0.6f, 1.0f), sf::Randomizer::Random(0.6f, 1.0f), sf::Randomizer::Random(0.6f, 1.0f)) {}

    void Star::draw() const {
        glColor3f(color_.r(),color_.g(),color_.b());
        glTexCoord2f(0, 0); glVertex2f(location_.x_-radius_, location_.y_-radius_);
        glTexCoord2f(0, 1); glVertex2f(location_.x_-radius_, location_.y_+radius_);
        glTexCoord2f(1, 1); glVertex2f(location_.x_+radius_, location_.y_+radius_);
        glTexCoord2f(1, 0); glVertex2f(location_.x_+radius_, location_.y_-radius_);
    }

    namespace {
        std::vector<Star> starVector_;
    }

    void draw() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Star));
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_QUADS);

        for (std::vector<Star>::iterator it = starVector_.begin(); it != starVector_.end(); ++it)
            it->draw();

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void populateSpace() {
        starVector_.clear();
        for (int i=0; i<settings::C_starCount; ++i)
            starVector_.push_back(Star());
    }
}
