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
        sf::Clock         clock_;
        sf::RenderTexture backBuffer_;
        sf::Sprite        fxImage_;

        Vector2f         viewPort_;
        float            scale_(static_cast<float>(settings::C_resX)/SPACE_X_RESOLUTION);
        int              clearCount_(0);
        float            joyButtonTimer_(0.f);
        const float      ratio(static_cast<float>(SPACE_X_RESOLUTION)/static_cast<float>(SPACE_Y_RESOLUTION));

        void setViewPort() {
            const int windowHeight(window_.getSize().y), windowWidth(window_.getSize().x);
            sf::View view(sf::FloatRect(0,0, windowWidth, windowHeight));
            if (static_cast<float>(windowWidth)/windowHeight > ratio) {

                view.setViewport(sf::FloatRect((windowWidth-viewPort_.x_)*0.5f / windowWidth, 0, 1, 1));
                glViewport((windowWidth-viewPort_.x_)*0.5f, 0, viewPort_.x_, viewPort_.y_);
            }
            else {
                view.setViewport(sf::FloatRect(0, (windowHeight-viewPort_.y_)*0.5f / windowHeight, 1, 1));
                glViewport(0, (windowHeight-viewPort_.y_)*0.5f, viewPort_.x_, viewPort_.y_);
            }

            window_.setView(view);
        }


        void resized() {
            window_.setActive(true);
            int windowHeight(window_.getSize().y), windowWidth(window_.getSize().x);
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
                backBuffer_.setActive(true);
                backBuffer_.clear();
                backBuffer_.create(viewPort_.x_, viewPort_.y_);
                glViewport(0,0,viewPort_.x_, viewPort_.y_);
                backBuffer_.setSmooth(false);
            }
        }

        void update() {
            timer::update(clock_.restart().asSeconds());
            sf::Event event;
            while (window_.pollEvent(event)) {
                if      (event.type == sf::Event::Resized)
                    resized();
                else if (event.type == sf::Event::Closed)
                    close();
                else if (event.type == sf::Event::KeyPressed) {
                    if (!menus::visible())
                        controllers::singleKeyEvent(Key(event.key.code));
                    menus::keyEvent(true, Key(event.key.code));
                }
                else if (event.type == sf::Event::KeyReleased) {
                    menus::keyEvent(false, Key(event.key.code));
                }
                else if (event.type == sf::Event::TextEntered) {
                    if (menus::visible())
                        menus::textEntered(event.text.unicode);
                }
                else if (event.type == sf::Event::MouseMoved) {
                    if (menus::visible())
                        menus::mouseMoved(Vector2f(event.mouseMove.x - (window_.getSize().x - viewPort_.x_)/2, event.mouseMove.y - (window_.getSize().y - viewPort_.y_)/2));
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (menus::visible() && event.mouseButton.button == sf::Mouse::Left)
                        menus::mouseLeft(true);
                }
                else if (event.type == sf::Event::MouseButtonReleased) {
                    if (menus::visible() && event.mouseButton.button == sf::Mouse::Left)
                        menus::mouseLeft(false);
                }
                else if (event.type == sf::Event::JoystickButtonPressed) {
                    if (timer::realTotalTime() - joyButtonTimer_ > 0.1f) {
                        if (!menus::visible())
                            controllers::singleKeyEvent(Key(event.joystickButton.joystickId, event.joystickButton.button));
                        menus::keyEvent(true, Key(event.joystickButton.joystickId, event.joystickButton.button));
                        joyButtonTimer_ = timer::realTotalTime();
                    }
                }
                else if (event.type == sf::Event::JoystickButtonReleased)
                    menus::keyEvent(false, Key(event.joystickButton.joystickId, event.joystickButton.button));
                else if (event.type == sf::Event::JoystickMoved) {
                    Key key(event.joystickMove.joystickId, event.joystickMove.axis, event.joystickMove.position);
                    if (key.strength_ >= 95 && timer::realTotalTime() - joyButtonTimer_ > 0.1f) {
                        if (!menus::visible())
                            controllers::singleKeyEvent(key);
                        if(key.strength_ >= 95)
                            menus::keyEvent(true, key);
                        joyButtonTimer_ = timer::realTotalTime();
                    }
                }
                else if (event.type == sf::Event::MouseWheelMoved) {
                    if (menus::visible())
                        menus::mouseWheelMoved(Vector2f(event.mouseWheel.x - (window_.getSize().x - viewPort_.x_)/2, event.mouseWheel.y - (window_.getSize().y - viewPort_.y_)/2), event.mouseWheel.delta);
                }
            }
        }

        void display() {
            window_.display();
            if (++clearCount_ > 30) {
                glClear(GL_COLOR_BUFFER_BIT);
                clearCount_ = 0;
            }
        }
    }

    // "public" methodes /////////////////////////////////////////////////

    bool open() {
        if (settings::load() && locales::load()) {
            postFX::load();

            create();
            return true;
        }
        else return false;
    }

    void close() {
        music::stop();
        window_.close();
    }

    void mainLoop() {
        while (window_.isOpen()) {
            update();
            if (window_.isOpen()) {
                games::update();
                games::draw();
                display();
            }
        }
    }

    void create() {
        sf::VideoMode mode(settings::C_resX, settings::C_resY, settings::C_colorDepth);

        if (settings::C_fullScreen && mode.isValid())
            window_.create(mode, "M.A.R.S. - a " + generateName::game(), sf::Style::Fullscreen);
        else
            window_.create(mode, "M.A.R.S. - a " + generateName::game());
        window_.setVerticalSyncEnabled(settings::C_vsync);
        //window_.SetFramerateLimit(15);

        # ifndef __APPLE__
            // apple uses bundle icon instead
            sf::Image icon;
            icon.loadFromFile(settings::C_dataPath + "tex/icon.png");
            window_.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
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
            backBuffer_.setActive(true);

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
            backBuffer_.display();

            window_.setActive(true);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            setViewPort();

            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            fxImage_.setTexture(backBuffer_.getTexture(), true);

            sf::Shader* shader = postFX::get();
            draw(fxImage_, sf::RenderStates(sf::BlendNone), shader);

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            setViewPort();

            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glEnable(GL_BLEND);
        }
        else {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.f, viewPort_.x_, viewPort_.y_, 0.f);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }
    }

    void draw(sf::Drawable const& toBeDrawn, sf::RenderStates const& states, sf::Shader* shader) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushMatrix();
        window_.pushGLStates();
        window_.setActive(true);
        glEnable(GL_TEXTURE_2D);

        sf::Shader::bind(shader);

        window_.draw(toBeDrawn, states);

        sf::Shader::bind(NULL);

        window_.popGLStates();
        glPopMatrix();
        glPopAttrib();
    }

    int isKeyDown(Key const& key) {
        switch (key.type_) {
            case Key::kKeyBoard:
                if (sf::Keyboard::isKeyPressed(key.code_.keyBoard_))
                    return 100;
                break;

            case Key::kJoyButton:
                if (sf::Joystick::isButtonPressed(key.joyID_, key.code_.joyButton_))
                    return 100;
                break;

            case Key::kJoyAxis:
                sf::Joystick::Axis tmp(Key::convertToSFML(key.code_.joyAxis_));
                int strength(sf::Joystick::getAxisPosition(key.joyID_, tmp));
                std::pair<Key::AxisType, int> result(Key::convertFromSFML(tmp,strength));
                return result.first == key.code_.joyAxis_ ? result.second : 0;
                break;
        }
        return 0;
    }

    Vector2f const getMousePosition() {
        return Vector2f(sf::Mouse::getPosition(window_).x, sf::Mouse::getPosition(window_).y);
    }

    void screenShot() {
        sf::Image shot = window_.capture();
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
            if (shot.saveToFile(settings::C_configPath + "screenshots/" + filename.str())) {
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
        window_.setMouseCursorVisible(show);
    }

    Vector2f const coordToPixel(Vector2f const& spaceCoord) {
        return spaceCoord*scale_;
    }

    Vector2f const PixelToCoord(Vector2f const& screenCoord) {
        return Vector2f(screenCoord.x_ - (window_.getSize().x - viewPort_.x_)/2, screenCoord.y_ - (window_.getSize().y - viewPort_.y_)/2);
    }

    Vector2f const& getViewPort() {
        return viewPort_;
    }
}
