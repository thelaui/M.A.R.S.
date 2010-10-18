/* postFX.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# include <Shaders/postFX.hpp>

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "Media/texture.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

namespace postFX {

    namespace {
        sf::Shader      postFX_;
        sf::RenderImage bumpMap_;
    }

    void update() {
        if (settings::C_shaders) {

            bumpMap_.SetActive(true);
            bumpMap_.Clear(sf::Color(127, 0, 127));

            particles::drawHeat();

            bumpMap_.Display();
        }
    }

    sf::Shader* get() {
        return &postFX_;
    }

    bool supported() {
        return (sf::Shader::IsAvailable() && sf::RenderImage::IsAvailable());
    }

    void load() {
        if (supported()) {
            postFX_.LoadFromFile("data/shaders/bump.frag");
            bumpMap_.Create(640, 400);
            glViewport(0,0,640,400);
            gluOrtho2D(0, 1280, 800, 0);
            glEnable(GL_BLEND);
            postFX_.SetTexture("BumpMap", bumpMap_.GetImage());
        }
        else
            std::cout << "Shaders are not supported on your hardware! There will be no fancy graphics..." << std::endl;
    }
}




