/* text.cpp

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

# include "Media/text.hpp"

# include "System/window.hpp"
# include "Locales/locales.hpp"

# include <SFML/Graphics.hpp>
# include <SFML/OpenGL.hpp>

namespace text {

    namespace {
        void drawText(sf::String const& text, Vector2f const& location,
                      float size, int align, Color3f const& color, sf::Font* font) {

            sf::Text drawString(text, font ? *font : *font::getFont(), size);
            drawString.SetColor(color.sfColor());

            drawString.SetBlendMode(sf::Blend::Add);

            Vector2f loc(location);
            sf::FloatRect boundingBox = drawString.GetRect();

            if (align == TEXT_ALIGN_CENTER)
                loc.x_ -= static_cast<int>(boundingBox.Width*0.5f);
            else if ((align == TEXT_ALIGN_RIGHT && locales::getCurrentLocale().LTR_) || (align == TEXT_ALIGN_LEFT && !locales::getCurrentLocale().LTR_))
                loc.x_ -= static_cast<int>(boundingBox.Width);

            // prevent text from being outside of screen
            Vector2f const& port = window::getViewPort();
            if (loc.x_ < 0.f)                           loc.x_ = 0.f;
            if (loc.y_ < 0.f)                           loc.y_ = 0.f;
            if (loc.x_ + boundingBox.Width  > port.x_)  loc.x_ = port.x_ - static_cast<int>(boundingBox.Width);
            if (loc.y_ + boundingBox.Height > port.y_)  loc.y_ = port.y_ - static_cast<int>(boundingBox.Height);

            drawString.SetPosition(loc.x_, loc.y_);

            window::draw(drawString);
        }
    }

    void drawSpaceText(sf::String const& text, Vector2f const& location,
                       float size, int align, Color3f const& color, sf::Font* font) {

        drawScreenText(text, window::coordToPixel(location), size, align, color, font);
    }

    void drawMobileSpaceText(sf::String const& text, Vector2f const& location,
                             float size, int align, Color3f const& color, sf::Font* font) {

        drawText(text, window::coordToPixel(location), size, align, color, font);

    }

    void drawScreenText(sf::String const& text, Vector2f const& location,
                       float size, int align, Color3f const& color, sf::Font* font) {

        drawText(text, Vector2f(static_cast<int>(location.x_), static_cast<int>(location.y_)), size, align, color, font);
    }

    void drawFooText() {
        drawScreenText(sf::String("."), Vector2f(0.f, 0.f), 1.f, TEXT_ALIGN_LEFT, Color3f(0.f, 0.f, 0.f));
    }

    float getCharacterPos(sf::String const& text, int pos, float size, int align, sf::Font* font) {
        sf::Text drawString(text, font ? *font : *font::getFont(), size);
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
}


