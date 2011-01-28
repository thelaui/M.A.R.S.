/* Team.cpp

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

# include "Zones/zones.hpp"

# include "Zones/HomeZone.hpp"
# include "Zones/TeamZone.hpp"
# include "Zones/TacticalZone.hpp"
# include "Zones/TutorialZone.hpp"
# include "Zones/RasterZone.hpp"
# include "Teams/Team.hpp"
# include "SpaceObjects/spaceObjects.hpp"
# include "SpaceObjects/SpaceObject.hpp"
# include "Games/games.hpp"
# include "SpaceObjects/Home.hpp"
# include "SpaceObjects/balls.hpp"

# include <iostream>

namespace zones {

    namespace {
        Zone *homeL_(NULL), *homeR_(NULL), *teamL_(NULL), *teamR_(NULL);
        std::vector<TutorialZone*> tutorialZones_  = std::vector<TutorialZone*>();
        std::vector<TacticalZone*> tacticalZonesL_ = std::vector<TacticalZone*>();
        std::vector<TacticalZone*> tacticalZonesR_ = std::vector<TacticalZone*>();
        std::vector<RasterZone*> rasterZones_ = std::vector<RasterZone*>();
        float totalTacticalAreaL_(0), totalTacticalAreaR_(0);
        int lastZone_(0);
    }

    Zone* addTeamZone(Vector2f const& location) {
        if(location.x_ < 640)
            return teamL_ = new TeamZone(0);
        else
            return teamR_ = new TeamZone(1);
    }

    Zone* addHomeZone(Vector2f const& location) {
        if(location.x_ < 640)
            return homeL_ = new HomeZone(location);
        else
            return homeR_ = new HomeZone(location);
    }

    void addTutorialZone(Vector2f const& location, float radius) {
        tutorialZones_.push_back(new TutorialZone(location, radius));
    }

    void detectTacticalZones() {
        // devide space objects into left and right side
        std::vector<SpaceObject*> objectsLeft = std::vector<SpaceObject*>();
        std::vector<SpaceObject*> objectsRight = std::vector<SpaceObject*>();

        for (std::vector<SpaceObject*>::const_iterator it=spaceObjects::getObjects().begin(); it!=spaceObjects::getObjects().end(); ++it) {
            if ((*it)->location().x_ < 640 && *it != spaceObjects::getHomes()[0] && *it != spaceObjects::getHomes()[1])
                objectsLeft.push_back(*it);
            if ((*it)->location().x_ > 641 && *it != spaceObjects::getHomes()[0] && *it != spaceObjects::getHomes()[1])
                objectsRight.push_back(*it);
        }

        Vector2f homeConnection (spaceObjects::getHomes()[0]->location()-spaceObjects::getHomes()[1]->location());
        Vector2f normalHomeConnection (homeConnection.normalize());

        // detect tactical zones between planets
        for (unsigned int i=0; i<objectsLeft.size(); ++i ) {
            if (i+1 < objectsLeft.size()){
                for (unsigned int j=i+1; j<objectsLeft.size(); ++j) {
                    Vector2f objectConnection (objectsLeft[i]->location()-objectsLeft[j]->location());
                    float objectDistance (objectConnection.length());
                    Vector2f normalObjectConnection(objectConnection.normalize());
                    float tolerate (normalObjectConnection * normalHomeConnection);
                    if (-0.7f <= tolerate && 0.7f >= tolerate) {
                        float distance ((objectDistance - objectsLeft[i]->radius() - objectsLeft[j]->radius())/2.f);
                        TacticalZone* zone(NULL);
                        if (objectsLeft[i]->location().y_ > objectsLeft[j]->location().y_)
                            zone = new TacticalZone(objectsLeft[j]->location() + normalObjectConnection * (objectsLeft[j]->radius() + distance),
                                                    distance);
                        else
                            zone = new TacticalZone(objectsLeft[i]->location() - normalObjectConnection * (objectsLeft[i]->radius() + distance),
                                                    distance);
                        tacticalZonesL_.push_back(zone);
                        totalTacticalAreaL_ += distance * distance;
                    }
                }
            }
        }

        for (unsigned int i=0; i<objectsRight.size(); ++i ) {
            if (i+1 < objectsRight.size()){
                for (unsigned int j=i+1; j<objectsRight.size(); ++j) {
                    Vector2f objectConnection (objectsRight[i]->location()-objectsRight[j]->location());
                    float objectDistance (objectConnection.length());
                    Vector2f normalObjectConnection(objectConnection.normalize());
                    float tolerate (normalObjectConnection * normalHomeConnection);
                    if (-0.7f <= tolerate && 0.7f >= tolerate) {
                        float distance ((objectDistance - objectsRight[i]->radius() - objectsRight[j]->radius())/2.f);
                        TacticalZone* zone(NULL);
                        if (objectsRight[i]->location().y_ > objectsRight[j]->location().y_)
                            zone = new TacticalZone(objectsRight[j]->location() + normalObjectConnection * (objectsRight[j]->radius() + distance),
                                                    distance);
                        else
                            zone = new TacticalZone(objectsRight[i]->location() - normalObjectConnection * (objectsRight[i]->radius() + distance),
                                                    distance);
                        tacticalZonesR_.push_back(zone);
                        totalTacticalAreaR_ += distance * distance;
                    }
                }
            }
        }

        // detect tactical zones at "space borders"

        // left side of the field
        if (objectsLeft.size() >= 1) {
            SpaceObject *highestL(objectsLeft[0]), *lowestL(objectsLeft[0]);
            if (objectsLeft.size() > 1) {
                for (unsigned int i=1; i<objectsLeft.size(); ++i) {
                    if (objectsLeft[i]->location().y_ > highestL->location().y_)
                        highestL = objectsLeft[i];
                    else if (objectsLeft[i]->location().y_ < lowestL->location().y_)
                        lowestL = objectsLeft[i];
                    else if (objectsLeft[i]->location().y_ > highestL->location().y_ - 100.f && objectsLeft[i]->location().y_ < highestL->location().y_ + 100.f) {
                        if (objectsLeft[i]->location().x_ < highestL->location().x_)
                            highestL = objectsLeft[i];
                    }
                    else if (objectsLeft[i]->location().y_ > lowestL->location().y_ - 100.f && objectsLeft[i]->location().y_ < lowestL->location().y_ + 100.f) {
                        if (objectsLeft[i]->location().x_ < lowestL->location().x_)
                            lowestL = objectsLeft[i];
                    }
                }
            }
            // zone between border and object with highest y values
            Vector2f homeToHighestL((highestL->location() - spaceObjects::getHomes()[0]->location()).normalize());
            Vector2f directionH (-homeToHighestL.y_, homeToHighestL.x_);
            float lengthFactorH ((800.f - highestL->location().y_) / directionH.y_);
            Vector2f objectToBorderH(directionH * lengthFactorH);
            float distanceH((objectToBorderH.length() - highestL->radius())/2.f);
            Vector2f centerH(highestL->location() +  directionH * (highestL->radius() + distanceH));
            if (centerH.x_ + distanceH < 640)
                tacticalZonesL_.push_back(new TacticalZone(centerH, distanceH));
            else {
                float newDistanceH((800.f - highestL->location().y_ - highestL->radius())/2.f);
                tacticalZonesL_.push_back(new TacticalZone(highestL->location() + Vector2f(0.f, 1.f) * (highestL->radius() + newDistanceH), newDistanceH));
            }
            totalTacticalAreaL_ += distanceH * distanceH;

            // zone between border and object with lowest y values
            Vector2f homeToLowestL((lowestL->location() - spaceObjects::getHomes()[0]->location()).normalize());
            Vector2f directionL (homeToLowestL.y_, -homeToLowestL.x_);
            float lengthFactorL ((0.f - lowestL->location().y_) / directionL.y_);
            Vector2f objectToBorderL(directionL * lengthFactorL);
            float distanceL((objectToBorderL.length() - lowestL->radius())/2.f);
            Vector2f centerL(lowestL->location() +  directionL * (lowestL->radius() + distanceL));
            if (centerL.x_ + distanceL < 640)
                tacticalZonesL_.push_back(new TacticalZone(centerL, distanceL));
            else {
                float newDistanceH((lowestL->location().y_ - lowestL->radius())/2.f);
                tacticalZonesL_.push_back(new TacticalZone(lowestL->location() + Vector2f(0.f, -1.f) * (lowestL->radius() + newDistanceH), newDistanceH));
            }
            totalTacticalAreaL_ += distanceL * distanceL;
        }

        // right side of the field
        if (objectsRight.size() >= 1) {
            SpaceObject *highestR(objectsRight[0]), *lowestR(objectsRight[0]);
            if (objectsRight.size() > 1) {
                for (unsigned int i=1; i<objectsRight.size(); ++i) {
                    if (objectsRight[i]->location().y_ > highestR->location().y_)
                        highestR = objectsRight[i];
                    else if (objectsRight[i]->location().y_ < lowestR->location().y_)
                        lowestR = objectsRight[i];
                    else if (objectsRight[i]->location().y_ > highestR->location().y_ - 100.f && objectsRight[i]->location().y_ < highestR->location().y_ + 100.f) {
                        if (objectsRight[i]->location().x_ > highestR->location().x_)
                            highestR = objectsRight[i];
                    }
                    else if (objectsRight[i]->location().y_ > lowestR->location().y_ - 100.f && objectsRight[i]->location().y_ < lowestR->location().y_ + 100.f) {
                        if (objectsRight[i]->location().x_ > lowestR->location().x_)
                            lowestR = objectsRight[i];
                    }
                }
            }
            // zone between border and object with highest y values
            Vector2f homeToHighestR((highestR->location() - spaceObjects::getHomes()[1]->location()).normalize());
            Vector2f directionH (homeToHighestR.y_, -homeToHighestR.x_);
            float lengthFactorH ((800.f - highestR->location().y_) / directionH.y_);
            Vector2f objectToBorderH(directionH * lengthFactorH);
            float distanceH((objectToBorderH.length() - highestR->radius())/2.f);
            Vector2f centerH(highestR->location() +  directionH * (highestR->radius() + distanceH));
            if (centerH.x_ + distanceH > 641)
                tacticalZonesR_.push_back(new TacticalZone(centerH, distanceH));
            else {
                float newDistanceH((800.f - highestR->location().y_ - highestR->radius())/2.f);
                tacticalZonesR_.push_back(new TacticalZone(highestR->location() + Vector2f(0.f, 1.f) * (highestR->radius() + newDistanceH), newDistanceH));
            }
            totalTacticalAreaR_ += distanceH * distanceH;

            // zone between border and object with lowest y values
            Vector2f homeToLowestR((lowestR->location() - spaceObjects::getHomes()[1]->location()).normalize());
            Vector2f directionL (-homeToLowestR.y_, homeToLowestR.x_);
            float lengthFactorL ((0.f - lowestR->location().y_) / directionL.y_);
            Vector2f objectToBorderL(directionL * lengthFactorL);
            float distanceL((objectToBorderL.length() - lowestR->radius())/2.f);
            Vector2f centerL(lowestR->location() +  directionL * (lowestR->radius() + distanceL));
            if (centerL.x_ + distanceL > 641)
                tacticalZonesR_.push_back(new TacticalZone(centerL, distanceL));
            else {
                float newDistanceH((lowestR->location().y_ - lowestR->radius())/2.f);
                tacticalZonesR_.push_back(new TacticalZone(lowestR->location() + Vector2f(0.f, -1.f) * (lowestR->radius() + newDistanceH), newDistanceH));
            }
            totalTacticalAreaR_ += distanceL * distanceL;
        }

        // adding tactical zones around home planets
        tacticalZonesL_.push_back(new TacticalZone(spaceObjects::getHomes()[0]->location(), 350.f));
        totalTacticalAreaL_ += 160000;
        tacticalZonesR_.push_back(new TacticalZone(spaceObjects::getHomes()[1]->location(), 350.f));
        totalTacticalAreaR_ += 160000;
    }

    void createRaster(int dimX, int dimY) {
        float maxX(0.f), maxY(0.f);
        for (int y=0; y<dimY; ++y) {
            for (int x=0; x<dimX; ++x) {
                if (!((x==0 && y==0) || (x==dimX-1 && y==0) || (x==0 && y==dimY-1) || (x==dimX-1 && y==dimY-1)))
                    rasterZones_.push_back(new RasterZone(Vector2f(maxX, maxY), Vector2f(maxX + 1280.f/dimX,  maxY + 800.f/dimY)));
                maxX += 1280.f/dimX;
            }
            maxX = 0;
            maxY += 800.f/dimY;
        }
    }

    void update() {
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesL_.begin(); it != tacticalZonesL_.end(); ++it)
            (*it)->update();
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesR_.begin(); it != tacticalZonesR_.end(); ++it)
            (*it)->update();
        for (std::vector<RasterZone*>::iterator it = rasterZones_.begin(); it != rasterZones_.end(); ++it)
            (*it)->update();
    }

    bool updateTutZones() {
        if (tutorialZones_.back()->isInside(*players::getPlayerI()->ship())) {
            delete tutorialZones_.back();
            tutorialZones_.pop_back();
        }

        return tutorialZones_.empty();
    }

    void draw() {
        if (games::type() != games::gDeathMatch && games::type() != games::gTutorial) {
            if (teamL_) teamL_->draw();
            if (teamR_) teamR_->draw();
            if (homeL_) homeL_->draw();
            if (homeR_) homeR_->draw();
        }
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesL_.begin(); it != tacticalZonesL_.end(); ++it)
            (*it)->draw();
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesR_.begin(); it != tacticalZonesR_.end(); ++it)
            (*it)->draw();
        if (games::type() != games::gTutorial)
            for (std::vector<RasterZone*>::iterator it = rasterZones_.begin(); it != rasterZones_.end(); ++it)
                (*it)->draw();
        for (std::vector<TutorialZone*>::iterator it = tutorialZones_.begin(); it != tutorialZones_.end(); ++it)
            (*it)->draw();
    }

    std::map<float, TacticalZone*> const toProtect(Team* checker) {
        std::map<float, TacticalZone*> sortedZones;
        Ball* ball = balls::getBall();
        if (ball) {
            Vector2f ballLocation(ball->location());
            if (checker->homeZone_ == homeL_)
                for (std::vector<TacticalZone*>::iterator it = tacticalZonesL_.begin(); it!=tacticalZonesL_.end(); ++it) {
                    sortedZones.insert(std::make_pair(((ballLocation + checker->home()->location())*0.5f - (*it)->location()).lengthSquare(), *it));
                }
            else
                for (std::vector<TacticalZone*>::iterator it = tacticalZonesR_.begin(); it!=tacticalZonesR_.end(); ++it) {
                    sortedZones.insert(std::make_pair(((ballLocation + checker->home()->location())*0.5f - (*it)->location()).lengthSquare(), *it));
                }
        }
        return sortedZones;
    }

    RasterZone* freeZone() {
        unsigned int count(0), i(lastZone_);
        while (++count < rasterZones_.size() && rasterZones_[(++i)%rasterZones_.size()]->covered())
            i %= rasterZones_.size();
        i %= rasterZones_.size();
        lastZone_ = count < rasterZones_.size() ? i : sf::Randomizer::Random(0, rasterZones_.size()-1);
        return rasterZones_[lastZone_];
    }

    float totalTacticalArea(short homeSide) {
        return homeSide == 0 ? totalTacticalAreaL_ : totalTacticalAreaR_;
    }

    short isInside(Team* checker, SpaceObject const& toBeChecked) {
        if (homeL_->isInside(toBeChecked)) return checker->homeZone_ == homeL_ ? OWN_HOME : ENEMY_HOME;
        if (homeR_->isInside(toBeChecked)) return checker->homeZone_ == homeR_ ? OWN_HOME : ENEMY_HOME;
        if (teamL_->isInside(toBeChecked)) return checker->teamZone_ == teamL_ ? OWN_TEAM : ENEMY_TEAM;
        if (teamR_->isInside(toBeChecked)) return checker->teamZone_ == teamR_ ? OWN_TEAM : ENEMY_TEAM;
        return NO_ZONE;
    }

    void clear() {
        if (homeL_) {
            delete homeL_;
            homeL_ = NULL;
        }
        if (homeR_) {
            delete homeR_;
            homeR_ = NULL;
        }
        if (teamL_) {
            delete teamL_;
            teamL_ = NULL;
        }
        if (teamR_) {
            delete teamR_;
            teamR_ = NULL;
        }
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesL_.begin(); it != tacticalZonesL_.end(); ++it)
            delete *it;
        for (std::vector<TacticalZone*>::iterator it = tacticalZonesR_.begin(); it != tacticalZonesR_.end(); ++it)
            delete *it;
        for (std::vector<RasterZone*>::iterator it = rasterZones_.begin(); it != rasterZones_.end(); ++it)
            delete *it;
        for (std::vector<TutorialZone*>::iterator it = tutorialZones_.begin(); it != tutorialZones_.end(); ++it)
            delete *it;
        tacticalZonesL_.clear();
        tacticalZonesR_.clear();
        rasterZones_.clear();
        tutorialZones_.clear();
    }

}
