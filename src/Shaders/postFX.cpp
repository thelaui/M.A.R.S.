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
        GLuint tex;
    }

    void update() {
        if (supported()) {
            const Vector2f viewport = window::getViewPort();
            postFXs_[Blur].SetParameter("radial_origin", sf::Vector2f(static_cast<float>(window::getInput().GetMouseX())/viewport.x_, 1.0 - static_cast<float>(window::getInput().GetMouseY())/viewport.y_));

            glEnable( GL_TEXTURE_2D );
            glBindTexture( GL_TEXTURE_2D, tex );
            glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, 960, 600);
            glDisable( GL_TEXTURE_2D );

        }
    }

    void draw() {
        if (supported()) {

            glEnable( GL_TEXTURE_2D );
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );


            //Motion-Faktor als Alpha-Faktor übergeben, Wert abhängig von Framerate ( z.B. 0.98 )
            glColor4d( 1.0f, 1.0f, 1.0f, 0.5f );

            //Motion-Textur über Szene blenden
            glBindTexture( GL_TEXTURE_2D, tex );

            Vector2f const& viewport = window::getViewPort();

            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex2f(0, 0);
                glTexCoord2f(0, 0); glVertex2f(0, 800);
                glTexCoord2f(1, 0); glVertex2f(1280, 800);
                glTexCoord2f(1, 1); glVertex2f(1280, 0);
            glEnd();

            glDisable( GL_TEXTURE_2D );


          /*  glViewport(0,0,480,300);
            glBindTexture(GL_TEXTURE_2D, tex);
            glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,480,300,0);
            glViewport(0,0,960,800);

            window::setPixelView();

            glDisable(GL_BLEND);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, tex);
            glBegin(GL_QUADS);

            Vector2f const& viewport = window::getViewPort();

            glColor3f(1.f, 1.0f, 1.0f);
            glTexCoord2f(0, 1); glVertex2f(0, 0);
            glTexCoord2f(0, 0); glVertex2f(0, viewport.y_);
            glTexCoord2f(1, 0); glVertex2f(viewport.x_, viewport.y_);
            glTexCoord2f(1, 1); glVertex2f(viewport.x_, 0);

            glEnd();

            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
            glEnable(GL_BLEND);

            window::setSpaceView();*/
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
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 960, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        if (supported()) {
            postFXs_[Sepia].LoadFromFile("data/shaders/sepia.frag");
            postFXs_[Blur].LoadFromFile("data/shaders/blur.frag");
        }
        else
            std::cout << "Shaders are not supported on your hardware! There will be no fancy graphics..." << std::endl;
    }
}




