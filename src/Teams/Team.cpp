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

# include "Teams/Team.hpp"

# include "Zones/zones.hpp"
# include "Players/Player.hpp"
# include "Players/BotPlayer.hpp"
# include "Players/LocalPlayer.hpp"
# include "SpaceObjects/Home.hpp"
# include "Controllers/KeyController.hpp"
# include "Games/games.hpp"
# include "Controllers/BotController.hpp"

Team::Team(Color3f const& color):
    evaluationTimer_(0.f),
    home_(NULL),
    color_(color),
    homeZone_(NULL),
    teamZone_(NULL),
    points_(0) {}

void Team::update() {
    evaluationTimer_ += timer::frameTime();
    if(evaluationTimer_ > 0.2f) {
        evaluationTimer_ = 0.f;
        evaluate();
    }
}

void Team::addMember(Player* player) {
    members_.push_back(player);
    player->team_ = this;

    if (player->type() == controllers::cBot) {
        BotPlayer* bot = dynamic_cast<BotPlayer*>(player);
        if (bot)
            botControllers_.push_back(bot->controller_);
    }
    else {
        LocalPlayer* localPlayer = dynamic_cast<LocalPlayer*>(player);
        if (localPlayer)
            keyControllers_.push_back(localPlayer->controller_);
    }
}

void Team::setHome(Home* home) {
    home_ = home;
    homeZone_ = zones::addHomeZone(home->location());
    teamZone_ = zones::addTeamZone(home->location());
}

void Team::addJob(Job const& job) {
    jobMap_.insert(std::make_pair(job, std::multimap<short, BotController*> ()));
}

void Team::resetPoints() const {
    points_ = 0;
}

void Team::addPoint() const {
    ++points_;
}

void Team::subtractPoint() const {
    --points_;
}

void Team::addStars() const {
    for(std::vector<Player*>::const_iterator it=members_.begin(); it!=members_.end(); ++it)
        ++(*it)->ship()->fragStars_;
}

void Team::evaluate() {
    jobMap_.clear();

    if (botControllers_.size() > 0 && games::elapsedTime() > 4.f) {
        createJobs();

        for (std::vector<BotController*>::iterator it=botControllers_.begin(); it!=botControllers_.end(); ++it)
            (*it)->evaluate();

        for (std::vector<KeyController*>::iterator it=keyControllers_.begin(); it!=keyControllers_.end(); ++it)
            (*it)->evaluate();

        for (std::vector<BotController*>::iterator it=botControllers_.begin(); it!=botControllers_.end(); ++it)
            (*it)->applyForJob(jobMap_);

        std::set<BotController*> unemployedBots(botControllers_.begin(), botControllers_.end());
        bool botsLeft(true);

        while (botsLeft) {
            std::multimap<Job, std::multimap<short, BotController*> >::iterator mostWantedJob;
            BotController* mostWanting=NULL;
            short highestDesire(0);

            for (std::multimap<Job, std::multimap<short, BotController*> >::iterator it=jobMap_.begin(); it!=jobMap_.end(); ++it) {
                std::multimap<short, BotController*>::reverse_iterator bot=it->second.rbegin();
                while (bot != it->second.rend() && unemployedBots.find(bot->second) == unemployedBots.end())
                    ++bot;
                if (bot != it->second.rend() && bot->first > highestDesire) {
                    highestDesire = bot->first;
                    mostWantedJob = it;
                    mostWanting   = bot->second;
                }
            }

            if (mostWanting) {
                unemployedBots.erase(mostWanting);
                mostWanting->assignJob(mostWantedJob->first);
                jobMap_.erase(mostWantedJob);
            }
            else {
                botsLeft = false;
            }
        }

        for (std::set<BotController*>::iterator it=unemployedBots.begin(); it!=unemployedBots.end(); ++it)
            (*it)->assignJob(Job(Job::jLand, 10));
    }
}







