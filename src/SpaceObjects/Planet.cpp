/* Planet.cpp

Copyright (c) 2010 by Felix Lauer und Simon Schneegans

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

# include "SpaceObjects/Planet.hpp"

# include "Particles/particles.hpp"
# include "DecoObjects/decoObjects.hpp"

Planet::Planet(Vector2f const& location, float radius):
               SpaceObject(spaceObjects::oPlanet, location, radius, radius*100),
               texture_(texture::randomPlanet()),
               color_(texture::getMudColor(texture_)) {
    physics::addStaticObject(this);
    physics::addGravitySource(this);

    if (sf::Randomizer::Random(0, 2) == 0)
        decoObjects::addPlanetSign(this);
}

void Planet::draw() const {
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture_));
    glColor3f(1,1,1);

    float drawRadius = radius_*1.7f;
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2f(location_.x_-drawRadius, location_.y_-drawRadius);
        glTexCoord2i(0, 1); glVertex2f(location_.x_-drawRadius, location_.y_+drawRadius);
        glTexCoord2i(1, 1); glVertex2f(location_.x_+drawRadius, location_.y_+drawRadius);
        glTexCoord2i(1, 0); glVertex2f(location_.x_+drawRadius, location_.y_-drawRadius);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Planet::onCollision(SpaceObject* with, Vector2f const& location,
                         Vector2f const& direction, Vector2f const& velocity) {
    float strength = velocity.length();

    switch (with->type()) {
        case spaceObjects::oAmmoROFLE:
            particles::spawnMultiple(20, particles::pMud, location, location-location_, velocity, color_);
            break;

        case spaceObjects::oAmmoShotgun:
            particles::spawnMultiple(2, particles::pMud, location, location-location_, velocity, color_);
            break;

        case spaceObjects::oAmmoAFK47:
            particles::spawnMultiple(2, particles::pMud, location, location-location_, velocity, color_);
            break;

        case spaceObjects::oShip:
            if (strength > 50)
                particles::spawnMultiple(10, particles::pMud, location, location-location_, velocity, color_);
            break;

        case spaceObjects::oAmmoBurner:
            particles::spawn(particles::pMiniFlame, location);
            break;

        default:;
    }
}
