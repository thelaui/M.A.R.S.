/* ColorPickerWindow.cpp

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

# include "Interface/ColorPickerWindow.hpp"

# include "Interface/ColorPicker.hpp"
# include "Interface/Button.hpp"
# include "Locales/locales.hpp"
# include "Menu/menus.hpp"
# include "Media/texture.hpp"
# include "System/window.hpp"

# include <SFML/OpenGL.hpp>

ColorPickerWindow::ColorPickerWindow (ColorPicker* parent, Color3f* color):
    UiWindow(190, 185),
    parent_(parent),
    color_(color),
    Ok_(new Button(locales::getLocale(locales::Ok), NULL, &kOk_, Vector2f(110, 155), 70, 20)),
    kOk_(false) {

    addWidget(Ok_);
}

void ColorPickerWindow::draw() const {
    UiWindow::draw();

    Vector2f viewPort = window::getViewPort();
    Vector2f origin;

    origin.x_ = topLeft_.x_ + (viewPort.x_ - width_)/2;
    origin.y_ = topLeft_.y_ + (viewPort.y_ - height_)/2;

    // update color according to mouse
    Vector2f mouse = window::PixelToCoord(window::getMousePosition());
    int x(mouse.x_), y(mouse.y_);
    if (sf::Mouse::IsButtonPressed(sf::Mouse::Left) && x>origin.x_ && x<origin.x_+190 && y>origin.y_ && y<origin.y_+150) {
        if (x>origin.x_+140) {
            float offset(y-origin.y_-10.f);
            if (offset > 119.9f) offset = 119.9f;
            if (offset < 0) offset = 0;
            color_->h(offset*3.f);
        }
        else {
            float offsetY(y-origin.y_-10.f);
            float offsetX(x-origin.x_-10.f);
            if (offsetY > 119.9f) offsetY = 119.9f;
            if (offsetY < 0) offsetY = 0;
            if (offsetX > 119.9f) offsetX = 119.9f;
            if (offsetX < 0) offsetX = 0;
            color_->s(1.f - offsetY/120.f);
            color_->v(0.001f + offsetX/120.f);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // draw b&w background grade
    glBegin(GL_QUADS);
        glColor3f(0.f, 0.f, 0.f);
        glVertex2f(origin.x_+10.f, origin.y_+10.f);
        glVertex2f(origin.x_+10.f, origin.y_+130.f);
        glColor3f(1.f, 1.f, 1.f);
        glVertex2f(origin.x_+130.f, origin.y_+130.f);
        glVertex2f(origin.x_+130.f, origin.y_+10.f);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture::getTexture(texture::Interface));

    // draw multicolored stripe
    glColor3f(1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.234375f, 0.390625f);   glVertex2f(origin.x_+150.f, origin.y_+10.f);
        glTexCoord2f(0.234375f, 0.625f);      glVertex2f(origin.x_+150.f, origin.y_+130.f);
        glTexCoord2f(0.29296875f, 0.625f);    glVertex2f(origin.x_+180.f, origin.y_+130.f);
        glTexCoord2f(0.29296875f, 0.390625f); glVertex2f(origin.x_+180.f, origin.y_+10.f);
    glEnd();

    // draw colored edge
    Color3f tmp(*color_);
    tmp.s(1.f);
    tmp.v(1.f);
    tmp.gl3f();
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0.390625f);      glVertex2f(origin.x_+10.f, origin.y_+10.f);
        glTexCoord2f(0, 0.625f);         glVertex2f(origin.x_+10.f, origin.y_+130.f);
        glTexCoord2f(0.234375f, 0.625f); glVertex2f(origin.x_+130.f, origin.y_+130.f);
        glTexCoord2f(0.234375f, 0.390625f);      glVertex2f(origin.x_+130.f, origin.y_+10.f);
    glEnd();

    // draw color selector
    float hue = color_->h();
    float sat = color_->s();
    float val = color_->v();

    tmp = *color_;
    tmp.s(1.f - sat);
    tmp.v(1.f - val);
    tmp.gl3f();
    Vector2f pickerLocation(origin + Vector2f(10.f + val*120.f, 130.f-sat*120.f));
    glBegin(GL_QUADS);
        glTexCoord2f(0.8359375f, 0.06640625f); glVertex2f(pickerLocation.x_+8.f, pickerLocation.y_+8.f);
        glTexCoord2f(0.8359375f, 0.09765625f); glVertex2f(pickerLocation.x_+8.f, pickerLocation.y_-8.f);
        glTexCoord2f(0.8671875f, 0.09765625f); glVertex2f(pickerLocation.x_-8.f, pickerLocation.y_-8.f);
        glTexCoord2f(0.8671875f, 0.06640625f); glVertex2f(pickerLocation.x_-8.f, pickerLocation.y_+8.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glColor3f(1.f, 1.f, 1.f);
    glLineWidth(2.f);
    glBegin(GL_LINES);
        glVertex2f(origin.x_+148.f, origin.y_+hue/3+10);
        glVertex2f(origin.x_+182.f, origin.y_+hue/3+10);
    glEnd();
}

void ColorPickerWindow::checkWidgets() {
    if (kOk_) {
        kOk_ = false;
        menus::hideWindow();
    }
}

