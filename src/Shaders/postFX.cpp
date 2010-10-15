/* postFX.cpp

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

# include <Shaders/postFX.hpp>

# include "System/window.hpp"

# include <SFML/OpenGL.hpp>
# include <SFML/Graphics.hpp>
# include <iostream>

namespace postFX {

    namespace {
        std::vector<sf::Shader> postFXs_(COUNT);
    }

    void update() {
        if (supported()) {
            const Vector2f viewport = window::getViewPort();
            postFXs_[Blur].SetParameter("radial_origin", sf::Vector2f(static_cast<float>(window::getInput().GetMouseX())/viewport.x_, 1.0 - static_cast<float>(window::getInput().GetMouseY())/viewport.y_));
        }
    }

    void activate  (EffectType type) {
        if (supported())
            postFXs_[type].Bind();
    }

    void deactivate(EffectType type) {
        if (supported())
            postFXs_[type].Unbind();
    }

    bool supported() {
        return (sf::Shader::IsAvailable() && sf::RenderImage::IsAvailable());
    }

    void load() {
        if (supported()) {
            postFXs_[Sepia].LoadFromFile("data/shaders/sepia.frag");
            postFXs_[Blur].LoadFromFile("data/shaders/blur.frag");
        }
        else
            std::cout << "Shaders are not supported on your hardware! There will be no fancy graphics..." << std::endl;
    }
}




