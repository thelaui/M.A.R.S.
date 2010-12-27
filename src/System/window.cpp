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
# include "Media/music.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>
# include <time.h>

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

        void screenShot() {
            sf::Image shot;
            const int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
            if (static_cast<float>(windowWidth)/windowHeight > 1.6f)
                shot.CopyScreen(window_, sf::IntRect((windowWidth-viewPort_.x_)*0.5f, 0, viewPort_.x_, viewPort_.y_));
            else
                shot.CopyScreen(window_, sf::IntRect(0, (windowHeight-viewPort_.y_)*0.5f, viewPort_.x_, viewPort_.y_));

            time_t rawtime;
            struct tm* timeinfo;
            time (&rawtime);
            timeinfo = localtime(&rawtime);

            std::stringstream filename;
            filename << "ScreenShot_" << timeinfo->tm_year << timeinfo->tm_mon << timeinfo->tm_mday << timeinfo->tm_hour << timeinfo->tm_min << timeinfo->tm_sec << ".jpg";

            if (shot.SaveToFile(settings::C_configPath + "screenshots/" + filename.str())) {
                std::cout << "Saved screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
            } else {
                std::cout << "Failed saving screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
            }
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
                    close();
                else if (event.Type == sf::Event::KeyPressed) {
                    if (event.Key.Code == settings::C_screenShotKey)
                        screenShot();
                    else if (!menus::visible())
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

    bool open() {
        if (settings::load()) {
            locales:: load();
            postFX::  load();
            fxImage_.SetBlendMode(sf::Blend::None);
            create();

            return true;
        }
        else return false;
    }

    void close() {
        music::stop();
        window_.Close();
    }

    void mainLoop() {
        while (isOpen()) {
            update();
            if (isOpen()) {
                games::update();
                games::draw();
                display();
            }
        }
    }

    void create() {
        sf::VideoMode mode(settings::C_resX, settings::C_resY, settings::C_colorDepth);

        if (settings::C_fullScreen && mode.IsValid())
            window_.Create(mode, "M.A.R.S. - a " + generateName::game(), sf::Style::Fullscreen);
        else
            window_.Create(mode, "M.A.R.S. - a " + generateName::game());

        window_.UseVerticalSync(settings::C_vsync);
        //window_.SetFramerateLimit(10);

        sf::Image icon;
        icon.LoadFromFile(settings::C_dataPath + "/tex/icon.png");
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

    Vector2f const coordToPixel(Vector2f const& spaceCoord) {
        return spaceCoord*scale_;
    }

    Vector2f const PixelToCoord(Vector2f const& screenCoord) {
        return Vector2f(screenCoord.x_ - (window_.GetWidth() - viewPort_.x_)/2, screenCoord.y_ - (window_.GetHeight() - viewPort_.y_)/2);
    }

    Vector2f const& getViewPort() {
        return viewPort_;
    }
}
