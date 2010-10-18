/* window.cpp

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

# include "System/window.hpp"

# include "System/generateName.hpp"
# include "System/settings.hpp"
# include "Media/texture.hpp"
# include "Controllers/controllers.hpp"
# include "Menu/menus.hpp"
# include "Locales/locales.hpp"
# include "System/timer.hpp"
# include "Shaders/postFX.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/stars.hpp"

# include <SFML/OpenGL.hpp>

namespace window {

    namespace {
        // main window of the game
        sf::RenderWindow window_;
        sf::RenderImage  backBuffer_;
        sf::Sprite       fxImage_;

        Vector2f         viewPort_;
        float            scale_(960.f/1280.f);
        int              clearCount_(0);

        void setViewPort() {
            const int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
            if (static_cast<float>(windowWidth)/windowHeight > 1.6f)
                glViewport((windowWidth-viewPort_.x_)*0.5f, 0, viewPort_.x_, viewPort_.y_);
            else
                glViewport(0, (windowHeight-viewPort_.y_)*0.5f, viewPort_.x_, viewPort_.y_);
        }

        void resized() {
            window_.SetActive(true);
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
            glClear(GL_COLOR_BUFFER_BIT);

            setViewPort();

            if (settings::C_shaders) {
                backBuffer_.SetActive(true);
                backBuffer_.Clear();
                backBuffer_.Create(viewPort_.x_, viewPort_.y_);
                glViewport(0,0,viewPort_.x_, viewPort_.y_);
                backBuffer_.SetSmooth(false);
            }
        }

        void update() {
            timer::update(window_.GetFrameTime());
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

        void display() {

            window_.Display();

            if (++clearCount_ > 30) {
                glClear(GL_COLOR_BUFFER_BIT);
                clearCount_ = 0;
            }

        }

         bool isOpen() {
            return window_.IsOpened();
        }

    }

    // "public" methodes /////////////////////////////////////////////////

    void open() {
        settings::load();
        locales:: load();
        postFX::  load();
        fxImage_.SetBlendMode(sf::Blend::None);
        create();
    }

    void close() {
        window_.Close();
    }

    void mainLoop() {
        while (isOpen()) {
            update();
            games::update();
            games::draw();
            display();
        }
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

        // probably improves performance...
        glDisable(GL_LIGHTING);
        glDisable(GL_POLYGON_SMOOTH);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_SCISSOR_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
        glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);

        glEnable(GL_BLEND);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POINT_SMOOTH);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void startDrawSpace() {
        if (settings::C_shaders)
            backBuffer_.SetActive(true);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        stars::draw();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.f, 1280.f, 800.f, 0.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    void startDrawHUD() {
        if (settings::C_shaders) {
            backBuffer_.Display();

            window_.SetActive(true);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            setViewPort();
            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            fxImage_.SetImage(backBuffer_.GetImage(), true);

            glEnable(GL_TEXTURE_2D);
            sf::Shader* shader = postFX::get();
            if (shader) window_.Draw(fxImage_, *shader);
            else        window_.Draw(fxImage_);
            glDisable(GL_TEXTURE_2D);
        }
        else {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            setViewPort();
            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
    }

    void draw(sf::Drawable const& toBeDrawn) {
        glEnable(GL_TEXTURE_2D);
        window_.Draw(toBeDrawn);
        glDisable(GL_TEXTURE_2D);
    }

    sf::Input const& getInput() {
        return window_.GetInput();
    }

    void showCursor(bool show) {
        window_.ShowMouseCursor(show);
    }

    void applyGlobalSettings() {
        if (settings::C_shaders) {
            backBuffer_.SetActive(true);
            backBuffer_.Clear();
            backBuffer_.Create(viewPort_.x_, viewPort_.y_);
            glViewport(0,0,viewPort_.x_, viewPort_.y_);
            backBuffer_.SetSmooth(false);
        }
        window_.UseVerticalSync(settings::C_vsync);
    }

    Vector2f const coordToPixel(Vector2f const& spaceCoord) {
        return spaceCoord*scale_;
    }

    Vector2f const& getViewPort() {
        return viewPort_;
    }
}
