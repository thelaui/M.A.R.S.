/* settings.hpp

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

# ifndef SETTINGS_HPP_INCLUDED
# define SETTINGS_HPP_INCLUDED

# include "Color3f.hpp"

# include <SFML/Graphics.hpp>
# include <string>

/// A namespace which handles the settings.
/// All settings are stored in global variables and
/// can be saved to and loaded from file

namespace settings {
    /// Loads all settings from the file.
    /// All settings are stored in the file mars.cfg in C_configPath.
    /// This has to be called once at the beginning of the game. If an error
    /// occurs, default settings are used.
    bool load();

    /// Saves the current state of the variables to file.
    bool save();

    /// \name Game settings
    ///@{
    extern int            C_soundVolume;
    extern int            C_announcerVolume;
    extern int            C_musicVolume;
    extern bool           C_showFPS;
    extern bool           C_showLatency;
    extern bool           C_showParticleCount;
    extern bool           C_fullScreen;
    extern bool           C_vsync;
    extern bool           C_StarsHigh;
    extern bool           C_drawLocalNames;
    extern bool           C_drawRemoteNames;
    extern int            C_globalParticleCount;
    extern bool           C_adaptiveParticleCount;
    extern int            C_globalParticleLifeTime;
    extern int            C_botsLeft;
    extern int            C_botsRight;
    extern int            C_botsDeath;
    extern int            C_pointLimit;
    extern int            C_pointLimitDM;
    extern int            C_pointLimitTDM;
    extern int            C_powerUpRate;
    extern int            C_slowMoKickIn;
    extern bool           C_showInfoHide;
    extern bool           C_showInfoSB;
    extern bool           C_showInfoDM;
    extern bool           C_showInfoTDM;
    extern bool           C_showInfoCK;
    extern bool           C_showSelectLanguage;
    extern int            C_languageID;
    extern int            C_resX;
    extern int            C_resY;
    extern int            C_colorDepth;
    extern bool           C_shaders;
    extern sf::Key::Code  C_screenShotKey;
    extern std::string    C_configPath;
    extern std::string    C_dataPath;
    extern std::string    C_screenShotFormat;
    ///@}

    /// \name Player settings
    ///@{
    extern sf::String     C_playerIName;
    extern Color3f        C_playerIColor;
    extern Color3f        C_playerITeamColor;
    extern sf::Key::Code  C_playerIup;
    extern sf::Key::Code  C_playerIleft;
    extern sf::Key::Code  C_playerIright;
    extern sf::Key::Code  C_playerIfire;
    extern sf::Key::Code  C_playerIspecial;
    extern bool           C_playerIteamL;
    extern bool           C_playerIteamR;
    extern int            C_playerIShip;
    extern sf::String     C_playerIIName;
    extern Color3f        C_playerIIColor;
    extern Color3f        C_playerIITeamColor;
    extern sf::Key::Code  C_playerIIup;
    extern sf::Key::Code  C_playerIIleft;
    extern sf::Key::Code  C_playerIIright;
    extern sf::Key::Code  C_playerIIfire;
    extern sf::Key::Code  C_playerIIspecial;
    extern bool           C_playerIIteamL;
    extern bool           C_playerIIteamR;
    extern int            C_playerIIShip;
    extern bool           C_networkPlayerI;
    ///@}

    /// \name AI settings
    ///@{
    extern bool           C_drawBotNames;
    extern bool           C_drawBotOrientation;
    extern bool           C_drawZones;
    extern bool           C_drawAIPath;
    extern int            C_iDumb;
    extern bool           C_drawBotOrientation;
    extern bool           C_drawZones;
    ///@}

    /// \name Network settings
    ///@{
    extern sf::String    C_ip;
    extern sf::String    C_port;
    ///@}
};

# endif //SETTINGS_HPP_INCLUDED
