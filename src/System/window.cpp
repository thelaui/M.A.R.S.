/* window.cpp

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
# include "Hud/hud.hpp"
# include "defines.hpp"

# include <SFML/OpenGL.hpp>
# include <sstream>
# include <time.h>
# include <sys/stat.h>

# ifdef __WIN32__
    # include <windows.h>
# endif


namespace window {

    namespace {
        // main window of the game
        sf::RenderWindow  window_;
        sf::RenderTexture backBuffer_;
        sf::Sprite        fxImage_;

        Vector2f         viewPort_;
        float            scale_(static_cast<float>(settings::C_resX)/SPACE_X_RESOLUTION);
        int              clearCount_(0);
        float            joyButtonTimer_(0.f);
        const float      ratio(static_cast<float>(SPACE_X_RESOLUTION)/static_cast<float>(SPACE_Y_RESOLUTION));

        void setViewPort() {
            const int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
            if (static_cast<float>(windowWidth)/windowHeight > ratio) {
                glViewport((windowWidth-viewPort_.x_)*0.5f, 0, viewPort_.x_, viewPort_.y_);
            }
            else {
                glViewport(0, (windowHeight-viewPort_.y_)*0.5f, viewPort_.x_, viewPort_.y_);
            }
        }


        void resized() {
            window_.SetActive(true);
            int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
            // if windows aspect ration is greater than aspect ratio of space
            if (static_cast<float>(windowWidth)/windowHeight > ratio) {
                scale_ = static_cast<float>(windowHeight)/SPACE_Y_RESOLUTION;
                viewPort_.y_ = windowHeight;
                viewPort_.x_  = windowHeight * ratio;
            }
            else {
                scale_ = static_cast<float>(windowWidth)/SPACE_X_RESOLUTION;
                viewPort_.y_ = windowWidth / ratio;
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
            timer::update(window_.GetFrameTime()*0.001f);
            sf::Event event;
            while (window_.PollEvent(event)) {
                if      (event.Type == sf::Event::Resized)
                    resized();
                else if (event.Type == sf::Event::Closed)
                    close();
                else if (event.Type == sf::Event::KeyPressed) {
                    if (!menus::visible())
                        controllers::singleKeyEvent(Key(event.Key.Code));
                    menus::keyEvent(true, Key(event.Key.Code));
                }
                else if (event.Type == sf::Event::KeyReleased) {
                    menus::keyEvent(false, Key(event.Key.Code));
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
                else if (event.Type == sf::Event::JoystickButtonPressed) {
                    if (timer::realTotalTime() - joyButtonTimer_ > 0.1f) {
                        if (!menus::visible())
                            controllers::singleKeyEvent(Key(event.JoystickButton.JoystickId, event.JoystickButton.Button));
                        menus::keyEvent(true, Key(event.JoystickButton.JoystickId, event.JoystickButton.Button));
                        joyButtonTimer_ = timer::realTotalTime();
                    }
                }
                else if (event.Type == sf::Event::JoystickButtonReleased)
                    menus::keyEvent(false, Key(event.JoystickButton.JoystickId, event.JoystickButton.Button));
                else if (event.Type == sf::Event::JoystickMoved) {
                    Key key(event.JoystickMove.JoystickId, event.JoystickMove.Axis, event.JoystickMove.Position);
                    if (key.strength_ >= 95 && timer::realTotalTime() - joyButtonTimer_ > 0.1f) {
                        if (!menus::visible())
                            controllers::singleKeyEvent(key);
                        if(key.strength_ >= 95)
                            menus::keyEvent(true, key);
                        joyButtonTimer_ = timer::realTotalTime();
                    }
                }
                else if (event.Type == sf::Event::MouseWheelMoved) {
                    if (menus::visible())
                        menus::mouseWheelMoved(Vector2f(event.MouseWheel.X - (window_.GetWidth() - viewPort_.x_)/2, event.MouseWheel.Y - (window_.GetHeight() - viewPort_.y_)/2), event.MouseWheel.Delta);
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
    }

    // "public" methodes /////////////////////////////////////////////////

    bool open() {
        if (settings::load() && locales::load()) {
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
        while (window_.IsOpened()) {
            update();
            if (window_.IsOpened()) {
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
        window_.EnableVerticalSync(settings::C_vsync);
        //window_.SetFramerateLimit(15);

        # ifndef __APPLE__
            // apple uses bundle icon instead
            sf::Image icon;
            icon.LoadFromFile(settings::C_dataPath + "tex/icon.png");
            window_.SetIcon(icon.GetWidth(), icon.GetHeight(), icon.GetPixelsPtr());
        # endif

        resized();

        // setup OpenGL rendering context, bg color
        glClearColor(0.f, 0.f, 0.f, 0.f);

        // Edit the OpenGL projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        // Setup translation (according to left-upper corner)
        gluOrtho2D(0.f, SPACE_X_RESOLUTION, SPACE_Y_RESOLUTION, 0.f);

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
        gluOrtho2D(0.f, SPACE_X_RESOLUTION, SPACE_Y_RESOLUTION, 0.f);
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

            fxImage_.SetTexture(backBuffer_.GetTexture(), true);

            glEnable(GL_TEXTURE_2D);
            sf::Shader* shader = postFX::get();
            if (shader) window_.Draw(fxImage_, *shader);
            else        window_.Draw(fxImage_);
            glDisable(GL_TEXTURE_2D);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            setViewPort();

            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
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
        window_.SetActive(true);
        glEnable(GL_TEXTURE_2D);
        window_.Draw(toBeDrawn);
        glDisable(GL_TEXTURE_2D);
    }

    int isKeyDown(Key const& key) {
        switch (key.type_) {
            case Key::kKeyBoard:
                if (sf::Keyboard::IsKeyPressed(key.code_.keyBoard_))
                    return 100;
                break;

            case Key::kJoyButton:
                if (sf::Joystick::IsButtonPressed(key.joyID_, key.code_.joyButton_))
                    return 100;
                break;

            case Key::kJoyAxis:
                sf::Joystick::Axis tmp(Key::convertToSFML(key.code_.joyAxis_));
                int strength(sf::Joystick::GetAxisPosition(key.joyID_, tmp));
                std::pair<Key::AxisType, int> result(Key::convertFromSFML(tmp,strength));
                return result.first == key.code_.joyAxis_ ? result.second : 0;
                break;
        }
        return 0;
    }

    Vector2f const getMousePosition() {
        return Vector2f(sf::Mouse::GetPosition(window_).x, sf::Mouse::GetPosition(window_).y);
    }

    void screenShot() {
        sf::Image shot = window_.Capture();
       // const int windowHeight(window_.GetHeight()), windowWidth(window_.GetWidth());
       // if (static_cast<float>(windowWidth)/windowHeight > ratio)
       //     shot.Copy(window_, sf::IntRect((windowWidth-viewPort_.x_)*0.5f, 0, viewPort_.x_, viewPort_.y_));
       // else
       //     shot.Copy(window_, sf::IntRect(0, (windowHeight-viewPort_.y_)*0.5f, viewPort_.x_, viewPort_.y_));

        time_t rawtime;
        struct tm* timeinfo;
        time (&rawtime);
        timeinfo = localtime(&rawtime);

        std::stringstream filename;
        filename << "ScreenShot_" << timeinfo->tm_year << timeinfo->tm_mon << timeinfo->tm_mday << timeinfo->tm_hour << timeinfo->tm_min << timeinfo->tm_sec << "." << settings::C_screenShotFormat;

        # ifdef __linux__
            mkdir((settings::C_configPath + "screenshots/").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (shot.SaveToFile(settings::C_configPath + "screenshots/" + filename.str())) {
                std::cout << "Saved screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
                hud::displayMessage(*locales::getLocale(locales::SavedScreenshot));
            } else {
                std::cout << "Failed saving screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
            }
        # endif

        # ifdef __APPLE__
            mkdir((settings::C_configPath + "screenshots/").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if (shot.SaveToFile(settings::C_configPath + "screenshots/" + filename.str())) {
                std::cout << "Saved screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
                hud::displayMessage(*locales::getLocale(locales::SavedScreenshot));
            } else {
                std::cout << "Failed saving screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
            }
        # endif

        # ifdef __WIN32__
            CreateDirectory((settings::C_configPath + "screenshots/").c_str(), NULL);
            if (shot.SaveToFile(settings::C_configPath + "screenshots/" + filename.str())) {
                std::cout << "Saved screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
                hud::displayMessage(*locales::getLocale(locales::SavedScreenshot));
            } else {
                std::cout << "Failed saving screenshot to " << settings::C_configPath << "screenshots/" << filename.str() << "." << std::endl;
            }
        # endif
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
