/* window.cpp

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

# include "System/window.hpp"

# include "System/generateName.hpp"
# include "System/settings.hpp"
# include "Media/texture.hpp"
# include "Controllers/controllers.hpp"
# include "Menu/menus.hpp"

# include <SFML/OpenGL.hpp>

namespace window {

    namespace {
        // main window of the game
        sf::RenderWindow window_;
        Vector2f         viewPort_;
        bool             isCreated_(false);
        bool             resized_(false);
        float            scale_(960.f/1280.f);

        void setViewPort() {
            int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
            if (static_cast<float>(windowWidth)/windowHeight > 1.6f)
                glViewport((windowWidth-viewPort_.x_)/2, 0, viewPort_.x_, viewPort_.y_);
            else
                glViewport(0, (windowHeight-viewPort_.y_)/2, viewPort_.x_, viewPort_.y_);
        }
    }

    sf::RenderWindow* getMainWindow() {
        if (isCreated_)
            return &window_;
        else {
            // Create the main rendering window
            settings::load();
            create();
            isCreated_ = true;
            return &window_;
        }
    }

    bool isOpen() {
        return window_.IsOpened();
    }

    void handleEvents() {
        sf::Event event;
        while (window_.GetEvent(event)) {
            if      (event.Type == sf::Event::Resized)
                resized();
            else if (event.Type == sf::Event::Closed)
                window_.Close();
            else if (event.Type == sf::Event::KeyPressed) {
                if (!menus::visible())
                    controllers::singleKeyEvent(event.Key.Code);
                menus::buttonPressed(event.Key.Code);
            }
            else if (event.Type == sf::Event::TextEntered) {
                if (menus::visible())
                    menus::textEntered(event.Text.Unicode);
            }
            else if (event.Type == sf::Event::MouseMoved) {
                if (menus::visible())
                    menus::mouseMoved(Vector2f(event.MouseMove.X - (window_.GetWidth() - viewPort_.x_)/2, event.MouseMove.Y - (window_.GetHeight() - viewPort_.y_)/2));
            }
            else if (event.Type == sf::Event::MouseButtonPressed) {
                if (menus::visible() && event.MouseButton.Button == sf::Mouse::Left)
                    menus::mouseLeft(true);
            }
            else if (event.Type == sf::Event::MouseButtonReleased) {
                if (menus::visible() && event.MouseButton.Button == sf::Mouse::Left)
                    menus::mouseLeft(false);
            }
        }
    }

    void resized() {
        int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
        // if windows aspect ration is greater than aspect ratio of space
        if (static_cast<float>(windowWidth)/windowHeight > 1.6f) {
            scale_ = static_cast<float>(windowHeight)/800.f;
            viewPort_.y_ = windowHeight;
            viewPort_.x_  = windowHeight * 1.6f;
        }
        else {
            scale_ = static_cast<float>(windowWidth)/1280.f;
            viewPort_.y_ = windowWidth / 1.6f;
            viewPort_.x_  = windowWidth;
        }
        setViewPort();
        resized_ = true;
    }

    void setPixelView() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setViewPort();
        gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void setSpaceView() {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        setViewPort();
        gluOrtho2D(0.f, 1280.f, 800.f, 0.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void display() {
        window_.Display();
    }

    void clear() {
        if (!settings::C_showStars || resized_) {
            glClear(GL_COLOR_BUFFER_BIT);
            resized_ = false;
        }
    }

    void showCursor(bool show) {
        window_.ShowMouseCursor(show);
    }

    void create() {
        if (settings::C_fullScreen)
            window_.Create(sf::VideoMode::GetFullscreenModes()[0], "M.A.R.S. - a " + generateName::game(), sf::Style::Fullscreen);
        else
            window_.Create(sf::VideoMode(960, 600, 32), "M.A.R.S. - a " + generateName::game());

        window_.UseVerticalSync(settings::C_vsync);

        sf::Image icon;
        icon.LoadFromFile("data/tex/icon.png");
        window_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());

        resized();

        // setup OpenGL rendering context, bg color
        glClearColor(0.f, 0.f, 0.f, 0.f);

        // Edit the OpenGL projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Setup translation (according to left-upper corner)
        gluOrtho2D(0.f, 1280.f, 800.f, 0.f);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    Vector2f coordToPixel(Vector2f const& spaceCoord) {
        return spaceCoord*scale_;
    }

    Vector2f pixelToCoord(Vector2f const& pixelCoord) {
        return pixelCoord/scale_;
    }

    Vector2f const& getViewPort() {
        return viewPort_;
    }

    void close() {
        window_.Close();
    }
}
