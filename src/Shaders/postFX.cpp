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

# include "System/timer.hpp"

# include <iostream>

namespace postFX {

    namespace {
        sf::Shader postFX_;

        Vector2f exploLocation_;
        float    timer_(0.f);
    }

    void update() {
        if (timer_ > 0 && supported()) {
            timer_ -= timer::frameTime();
            postFX_.SetParameter("radial_blur", timer_*timer_*(1-std::sin(((1.5f-timer_)*2.5f)*M_PI)));
           // postFX_.SetParameter("radial_bright", (1.f-std::sin((1.f-timer_*2.f)*0.5f*M_PI))*2.f + 1.f);
        }
    }

    sf::Shader* get() {
        return (timer_ > 0.f) ? &postFX_ : NULL;
    }

    void spawnExplosion(Vector2f const& location) {
        exploLocation_ = location;
        postFX_.SetParameter("radial_origin", sf::Vector2f(location.x_/1280.f, 1.f - location.y_/800.f));
        timer_ = 1.5f;
    }

    bool supported() {
        return (sf::Shader::IsAvailable() && sf::RenderImage::IsAvailable());
    }

    void load() {
        if (supported()) {
            postFX_.LoadFromFile("data/shaders/blur.frag");
            postFX_.SetParameter("radial_blur", 0.f);
          //  postFX_.SetParameter("radial_bright", 1.f);
        }
        else
            std::cout << "Shaders are not supported on your hardware! There will be no fancy graphics..." << std::endl;
    }
}




