/* ShipName.cpp

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

# include "DecoObjects/ShipName.hpp"

# include "SpaceObjects/Ship.hpp"
# include "Players/Player.hpp"
# include "Media/text.hpp"
# include "System/settings.hpp"

# include <sstream>

void ShipName::draw() const {
    if (ship_->visible_) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        if (ship_->weaponChange_ && ((ship_->owner_->type() == controllers::cPlayer1) | (ship_->owner_->type() == controllers::cPlayer2)))
            text::drawSpaceText(ship_->currentWeapon_->getName(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, Color3f(0.8f, 0.8f, 0.8f));
        else {
            Color3f color(1.f, 0.f, 0.f);
            color.h(color.h() + ship_->getLife());
            if (settings::C_drawBotOrientation)
                switch (ship_->owner_->type()) {
                    case controllers::cAggroBot:
                        if (ship_->docked_)
                            text::drawSpaceText(ship_->owner_->name() + " [AGGRO]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        else
                            text::drawMobileSpaceText(ship_->owner_->name() + " [AGGRO]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        break;
                    case controllers::cDefBot:
                        if (ship_->docked_)
                            text::drawSpaceText(ship_->owner_->name() + " [DEF]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        else
                            text::drawMobileSpaceText(ship_->owner_->name() + " [DEF]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        break;
                    case controllers::cMidBot:
                        if (ship_->docked_)
                            text::drawSpaceText(ship_->owner_->name() + " [MID]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        else
                            text::drawMobileSpaceText(ship_->owner_->name() + " [MID]", ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        break;
                    default:
                        if (ship_->docked_)
                            text::drawSpaceText(ship_->owner_->name(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                        else
                            text::drawMobileSpaceText(ship_->owner_->name(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                }
            else {
                if (ship_->docked_)
                    text::drawSpaceText(ship_->owner_->name(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
                else
                    text::drawMobileSpaceText(ship_->owner_->name(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f, font::HandelGotDLig, 12.f, TEXT_ALIGN_CENTER, color);
            }
        }

        if (ship_->fragStars_ > 0) {
            Color3f color;
            int showAmount;
            if (ship_->fragStars_ > 10) {
                color = Color3f(1.f, 0.8f, 0.f);
                showAmount = ship_->fragStars_ -10;
            }
            else if (ship_->fragStars_ > 5) {
                color = Color3f(0.8f, 0.8f, 0.8f);
                showAmount = ship_->fragStars_-5;
            }
            else {
                color = Color3f(1.0f, 0.5f, 0.7f);
                showAmount = ship_->fragStars_;
            }

            std::stringstream sstr;
            for (int i=0; i<showAmount; ++i)
                sstr << "*";
            if (ship_->docked_)
                text::drawSpaceText(sstr.str(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f + Vector2f(0.f, -17.f), font::HandelGotDLig, 25.f, TEXT_ALIGN_CENTER, color);
            else
                text::drawMobileSpaceText(sstr.str(), ship_->location_ + Vector2f(0.f, -ship_->radius_)*2.5f + Vector2f(0.f, -17.f), font::HandelGotDLig, 25.f, TEXT_ALIGN_CENTER, color);
        }
    }
}
