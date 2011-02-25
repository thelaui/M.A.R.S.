/* postFX.cpp

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

# include <Shaders/postFX.hpp>

# include "System/timer.hpp"
# include "System/settings.hpp"
# include "Particles/particles.hpp"
# include "DecoObjects/decoObjects.hpp"
# include "Media/texture.hpp"
# include "defines.hpp"

# include <SFML/OpenGL.hpp>
# include <iostream>

namespace postFX {

    namespace {
        sf::Shader      postFX_;
        sf::RenderImage bumpMap_;
        float           exposure_(1.f);
        float           flashTimer_(0.f);
    }

    void update() {
        if (settings::C_shaders) {

            bumpMap_.SetActive(true);
            bumpMap_.Clear(sf::Color(127, 0, 127));

            particles::drawHeat();
            decoObjects::drawHeat();

            bumpMap_.Display();

            if (flashTimer_ > 0) {
                flashTimer_ -= timer::frameTime();
                if (flashTimer_ > 0.4f)
                    exposure_ = (0.5f-flashTimer_)*5.f + 1.f;
                else if (flashTimer_ > 0)
                    exposure_ = (flashTimer_*1.25f) + 1;
                else
                    exposure_ = 1.f;
            }

            postFX_.SetParameter("Exposure", exposure_);
        }
    }

    void onExplosion() {
        flashTimer_ = 0.5f;
    }

    sf::Shader* get() {
        return &postFX_;
    }

    bool supported() {
        return (sf::Shader::IsAvailable() && sf::RenderImage::IsAvailable());
    }

    void load() {
        if (supported()) {
            postFX_.LoadFromFile(settings::C_dataPath + "shaders/bump.frag");
            bumpMap_.Create(SPACE_X_RESOLUTION*0.5f, SPACE_Y_RESOLUTION*0.5f);
            glViewport(0,0,SPACE_X_RESOLUTION*0.5f,SPACE_Y_RESOLUTION*0.5f);
            gluOrtho2D(0, SPACE_X_RESOLUTION, SPACE_Y_RESOLUTION, 0);
            glEnable(GL_BLEND);
            glMatrixMode(GL_MODELVIEW);
            postFX_.SetTexture("BumpMap", bumpMap_.GetImage());
            postFX_.SetParameter("Exposure", exposure_);
        }
        else
            std::cout << "Shaders are not supported on your hardware! There will be no fancy graphics..." << std::endl;
    }
}




