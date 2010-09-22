/* hud.cpp

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

# include "Hud/hud.hpp"

# include "System/window.hpp"
# include "SpaceObjects/ships.hpp"
# include "SpaceObjects/Home.hpp"
# include "Games/games.hpp"
# include "Menu/menus.hpp"
# include "Hud/Logo.hpp"
# include "Hud/LeftLife.hpp"
# include "Hud/RightLife.hpp"
# include "Hud/GameStats.hpp"
# include "Hud/GamePoints.hpp"
# include "Hud/TabStats.hpp"

# include <sstream>
# include <SFML/Graphics.hpp>

namespace hud {

    namespace {
        Logo* logo_ = new Logo();
        LeftLife* leftLife_ = new LeftLife();
        RightLife* rightLife_ = new RightLife();
        GameStats* gameStats_ = new GameStats();
        GamePoints* gamePoints_ = new GamePoints();
        TabStats* tabStats_ = new TabStats();
    }

    void update() {
        gamePoints_->update();
        if (window::getMainWindow()->GetInput().IsKeyDown(sf::Key::Tab)) {
            tabStats_->display();
            gamePoints_->display();
        }
        else tabStats_->display(false);

        tabStats_->update();
    }

    void draw() {
        ships::drawHighLights();
        window::setPixelView();
        if (games::type() == games::gMenu) logo_->draw();
        gameStats_->draw();

        if (!menus::visible()) {
            ships::drawNames();
            if(games::type() == games::gSpaceBall || games::type() == games::gCannonKeep) {
                std::vector<Home*> const& homes = spaceObjects::getHomes();
                for (std::vector<Home*>::const_iterator it = homes.begin(); it != homes.end(); ++it)
                    (*it)->drawLife();
                gamePoints_->draw();
            }
            rightLife_->draw();
            leftLife_ ->draw();
        }
        tabStats_->draw();
        window::setSpaceView();
    }

    void drawSpaceText(std::string const& text, Vector2f const& location, font::FontType type,
                       float size, int align, Color3f const& color, Vector2f const& velocity) {

        sf::Text drawString(text, font::getFont(type), size);
        drawString.SetColor(color.sfColor());

        Vector2f loc = window::coordToPixel(location);
        sf::FloatRect boundingBox = drawString.GetRect();

        if      (align == TEXT_ALIGN_CENTER) loc -= Vector2f(boundingBox.Width*0.5f, 0.f);
        else if (align == TEXT_ALIGN_RIGHT)  loc -= Vector2f(boundingBox.Width, 0.f);

        // prevent text from being outside of screen
        Vector2f const& port = window::getViewPort();
        if (loc.x_ < 0.f)                             loc.x_ = 0.f;
        if (loc.y_ < 0.f)                             loc.y_ = 0.f;
        if (loc.x_ + boundingBox.Width  > port.x_)  loc.x_ = port.x_ - boundingBox.Width;
        if (loc.y_ + boundingBox.Height > port.y_)  loc.y_ = port.y_ - boundingBox.Height;

        if (velocity == Vector2f()) drawString.SetPosition(static_cast<int>(loc.x_), static_cast<int>(loc.y_));
        else                        drawString.SetPosition(loc.x_, loc.y_);

        sf::RenderWindow* mainWindow = window::getMainWindow();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_TEXTURE_2D);
        mainWindow->Draw(drawString);
        glDisable(GL_TEXTURE_2D);
    }

    void drawScreenText(std::string const& text, Vector2f const& location, font::FontType type,
                       float size, int align, Color3f const& color) {

        sf::Text drawString(text, font::getFont(type), size);
        drawString.SetColor(color.sfColor());

        switch (align) {
            case TEXT_ALIGN_CENTER: {
                sf::FloatRect boundingBox = drawString.GetRect();
                drawString.SetPosition(static_cast<int>(location.x_-boundingBox.Width*0.5f), static_cast<int>(location.y_));
                break;
            }
            case TEXT_ALIGN_RIGHT: {
                sf::FloatRect boundingBox = drawString.GetRect();
                drawString.SetPosition(static_cast<int>(location.x_-boundingBox.Width), static_cast<int>(location.y_));
                break;
            }
            default:
                drawString.SetPosition(static_cast<int>(location.x_), static_cast<int>(location.y_));
        }
        sf::RenderWindow* mainwindow = window::getMainWindow();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glEnable(GL_TEXTURE_2D);
        mainwindow->Draw(drawString);
        glDisable(GL_TEXTURE_2D);
    }

    float getCharacterPos(std::string const& text, int pos, font::FontType type, float size, int align) {
        sf::Text drawString(text, font::getFont(type), size);
        float result = drawString.GetCharacterPos(pos).x;

        switch (align) {
            case TEXT_ALIGN_CENTER: {
                sf::FloatRect boundingBox = drawString.GetRect();
                result -= boundingBox.Width*0.5f;
                break;
            }
            case TEXT_ALIGN_RIGHT: {
                sf::FloatRect boundingBox = drawString.GetRect();
                result -= boundingBox.Width;
                break;
            }
            default: break;
        }
        return result;
    }

    void displayPoints() {
        gamePoints_->display();
    }

    void displayStats(bool show) {
        gamePoints_->display();
        tabStats_->display(show);
    }
    void refreshTabStats() {
        tabStats_->refresh();
    }
}

