/* Sun.cpp

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

# include "SpaceObjects/Sun.hpp"

# include "Media/sound.hpp"
# include "System/timer.hpp"
# include "Particles/particles.hpp"

Sun::Sun(Vector2f const& location, float radius):
         SpaceObject(spaceObjects::oSun,location, radius, radius*50),
         eruptionTimer_(0) {
    physics::addStaticObject(this);
    physics::addGravitySource(this);
}

void Sun::update() {
    if (eruptionTimer_ > 0) {
        float time = timer::frameTime();
        eruptionTimer_ -= time;
    }
    else {
        // get random direction
        Vector2f direction = Vector2f::randDir();
        Vector2f eruptionLocation = location_ + direction * radius_ * 0.9;
        float intensity = sf::Randomizer::Random(0.5f, 2.0f);
        particles::spawnMultiple(intensity*20, particles::pEruption, eruptionLocation, direction, direction*intensity);
        eruptionTimer_ = sf::Randomizer::Random(0.5f, 1.5f);
    }
}

void Sun::draw() const {
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Sun1));

    glColor3f(1,1,1);
    float drawRadius = radius_*2;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(location_.x_-drawRadius, location_.y_-drawRadius);
        glTexCoord2i(0, 1); glVertex2f(location_.x_-drawRadius, location_.y_+drawRadius);
        glTexCoord2i(1, 1); glVertex2f(location_.x_+drawRadius, location_.y_+drawRadius);
        glTexCoord2i(1, 0); glVertex2f(location_.x_+drawRadius, location_.y_-drawRadius);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Sun::onCollision(SpaceObject* with, Vector2f const& location,
                      Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();

    if (with->type() != spaceObjects::oFuel && strength > 50)
        particles::spawnMultiple(5, particles::pMud, location, direction, velocity, Color3f(1.0f, 0.9f, 0.2f));

    if (((with->type() == spaceObjects::oShip) | (with->type() == spaceObjects::oBall)) && (strength > 50)) {
        Vector2f direction = location - location_;
        Vector2f eruptionLocation = location_ + direction * 0.9;
        particles::spawnMultiple(strength*0.1, particles::pEruption, eruptionLocation, direction, direction*strength*0.00005);
        eruptionTimer_ = sf::Randomizer::Random(0.5f, 1.5f);
    }
}
