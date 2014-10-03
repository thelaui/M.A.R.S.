/* settings.cpp

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

# include "System/settings.hpp"

# include "Media/file.hpp"
# include "Shaders/postFX.hpp"
# include "defines.hpp"

# include <sys/stat.h>

# include <fstream>
# include <sstream>
# include <iostream>

# ifdef __WIN32__
    # include <shlobj.h>
    # include <windows.h>
# endif

inline int clamp(int x, int min, int max) {
    return x < min ? min : (x > max ? max : x);
}

namespace settings {
    // game settings ------ adjustable via options menu
    int         C_soundVolume =             30;
    int         C_announcerVolume =          0;
    int         C_musicVolume =             60;
    bool        C_showFPS =                 false;
    bool        C_showParticleCount =       false;
    bool        C_showLatency =             false;
    bool        C_fullScreen =              false;
    bool        C_vsync =                   true;
    bool        C_adaptiveParticleCount =   false;
    int         C_globalParticleCount =     100;
    int         C_globalParticleLifeTime =  100;
    bool        C_StarsHigh =               true;
    int         C_StarField =               500;
    int         C_botsLeft =                4;
    int         C_botsRight =               4;
    int         C_botsDeath =               10;
    int         C_pointLimitSB =            10;
    int         C_pointLimitCK =            10;
    int         C_pointLimitDM =            20;
    int         C_pointLimitTDM =           50;
    int         C_powerUpRate =             40;
    int         C_slowMoKickIn =            3;
    int         C_gameSpeed =               100;
    bool        C_showInfoHide =            true;
    bool        C_showInfoSB =              true;
    bool        C_showInfoDM =              true;
    bool        C_showInfoTDM =             true;
    bool        C_showInfoCK =              true;
    bool        C_showSelectLanguage =      true;
    bool        C_showToolTips =            true;
    int         C_languageID =              0;
    int         C_resX =                    INITIAL_WINDOW_X;
    int         C_resY =                    INITIAL_WINDOW_Y;
    int         C_colorDepth =              32;
    bool        C_shaders =                 false;
    Key         C_screenShotKey =           Key(sf::Keyboard::F12);
    bool        C_audioRandom =             true;
    Key         C_audioNextKey =            Key(sf::Keyboard::F8);
    Key         C_audioPreviousKey =        Key(sf::Keyboard::F7);
    Key         C_statisticsKey =           Key(sf::Keyboard::Tab);
    std::string C_configPath =              "";
    std::string C_dataPath =                "";
    std::string C_screenShotFormat =        "jpg";
    int         C_EnabledWeapons =          weapons::wAFK47 | weapons::wShotgun | weapons::wFlubba | weapons::wFist |
                                            weapons::wRocketLauncher | weapons::wROFLE | weapons::wBurner | weapons::wH2OMG;
    int         C_EnabledSpecials =         specials::sBlast | specials::sFreeze | specials::sHeal | specials::sFireWall |
                                            specials::sShocker;
    int         C_EnabledWeaponsByUser =    C_EnabledWeapons;
    int         C_EnabledSpecialsByUser =   C_EnabledSpecials;

    // player settings ----- adjustable via options menu
    sf::String    C_playerIName =           "PlayerI";
    Color3f       C_playerIColor =          Color3f(1.f, 0.87f, 0.0125f);
    Color3f       C_playerITeamColor =      Color3f(0.94f, 0.24f, 1.f);
    Key           C_playerIup =             Key(sf::Keyboard::Up);
    Key           C_playerIleft =           Key(sf::Keyboard::Left);
    Key           C_playerIright =          Key(sf::Keyboard::Right);
    Key           C_playerIfire =           Key(sf::Keyboard::RControl);
    Key           C_playerISpecialKey =     Key(sf::Keyboard::RShift);
    bool          C_playerIteamL =          false;
    bool          C_playerIteamR =          true;
    int           C_playerIShip =           0;
    weapons::WeaponType   C_playerIWeapon =  weapons::wAFK47;
    specials::SpecialType C_playerISpecial = specials::sHeal;
    sf::String    C_playerIIName =          "PlayerII";
    Color3f       C_playerIIColor =         Color3f(0.5f, 0.4f, 0.82f);
    Color3f       C_playerIITeamColor =     Color3f(0.05f, 1.f, 0.785f);
    Key           C_playerIIup =            Key(sf::Keyboard::W);
    Key           C_playerIIleft =          Key(sf::Keyboard::A);
    Key           C_playerIIright =         Key(sf::Keyboard::D);
    Key           C_playerIIfire =          Key(sf::Keyboard::LControl);
    Key           C_playerIISpecialKey =    Key(sf::Keyboard::LShift);
    bool          C_playerIIteamL =         true;
    bool          C_playerIIteamR =         false;
    int           C_playerIIShip =          0;
    weapons::WeaponType   C_playerIIWeapon =  weapons::wAFK47;
    specials::SpecialType C_playerIISpecial = specials::sHeal;
    bool          C_networkPlayerI =        true;

    // ai settings ------ adjustable via options menu
    bool        C_drawBotJobs =             false;
    bool        C_drawZones =               false;
    bool        C_drawAIPath =              false;
    int         C_iDumb =                   70;

    // network settings ----- adjustable via options menu
    sf::String C_ip =                      "192.168.0.1";
    sf::String C_port =                    "12345";

    bool save() {
        std::ofstream outStream((C_configPath + "mars.cfg").c_str());

        if (outStream) {

            outStream << "// mars config file" << std::endl;
            outStream << "// nearly all these options can be changed with the in-game menu, too." << std::endl << std::endl;

            outStream << "[soundVolume] "           <<  C_soundVolume << std::endl;
            outStream << "[announcerVolume] "       <<  C_announcerVolume << std::endl;
            outStream << "[musicVolume] "           <<  C_musicVolume << std::endl;
            outStream << "[globalParticleCount] "   <<  C_globalParticleCount << std::endl;
            outStream << "[globalParticleLifeTime] "<<  C_globalParticleLifeTime << std::endl;
            outStream << "[showFPS] "               << (C_showFPS ? "true" : "false") << std::endl;
            outStream << "[showParticleCount] "     << (C_showParticleCount ? "true" : "false") << std::endl;
            outStream << "[showLatency] "           << (C_showLatency ? "true" : "false") << std::endl;
            outStream << "[fullScreen] "            << (C_fullScreen ? "true" : "false") << std::endl;
            outStream << "[vsync] "                 << (C_vsync ? "true" : "false") << std::endl;
            outStream << "[drawAIPath] "            << (C_drawAIPath ? "true" : "false") << std::endl;
            outStream << "[iDumb] "                 << (C_iDumb) << std::endl;
            outStream << "[adaptiveParticleCount] " << (C_adaptiveParticleCount ? "true" : "false") << std::endl;
            outStream << "[drawBotJobs] "           << (C_drawBotJobs ? "true" : "false") << std::endl;
            outStream << "[drawZones] "             << (C_drawZones ? "true" : "false") << std::endl;
            outStream << "[botsLeft] "              << C_botsLeft << std::endl;
            outStream << "[botsRight] "             << C_botsRight << std::endl;
            outStream << "[botsDeath] "             << C_botsDeath << std::endl;
            outStream << "[pointLimitSB] "          << C_pointLimitSB << std::endl;
            outStream << "[pointLimitCK] "          << C_pointLimitCK << std::endl;
            outStream << "[pointLimitDM] "          << C_pointLimitDM << std::endl;
            outStream << "[pointLimitTDM] "         << C_pointLimitTDM << std::endl;
            outStream << "[powerUpRate] "           << C_powerUpRate << std::endl;
            outStream << "[slowMoKickIn] "          << C_slowMoKickIn << std::endl;
            outStream << "[gameSpeed] "             << C_gameSpeed << std::endl;
            outStream << "[playerIName] ";
            int i(0);
            while (i < 12 && i < C_playerIName.getSize()) {
                outStream<<C_playerIName[i]<<" ";
                ++i;
            }
            outStream << "0" << std::endl;
            outStream << "[playerIKeys] "           <<  C_playerIup << " "<< C_playerIright << " " << C_playerIleft << " " << C_playerIfire << " " << C_playerISpecialKey << std::endl;
            outStream << "[playerIColor] "          <<  C_playerIColor.r() << " "<< C_playerIColor.g() << " " << C_playerIColor.b() << std::endl;
            outStream << "[playerITeamColor] "      <<  C_playerITeamColor.r() << " "<< C_playerITeamColor.g() << " " << C_playerITeamColor.b() << std::endl;
            outStream << "[playerITeamL] "          << (C_playerIteamL ? "true" : "false") << std::endl;
            outStream << "[playerITeamR] "          << (C_playerIteamR ? "true" : "false") << std::endl;
            outStream << "[playerIShip] "           <<  C_playerIShip << std::endl;
            outStream << "[playerIWeapon] "         <<  C_playerIWeapon << std::endl;
            outStream << "[playerISpecial] "        <<  C_playerISpecial << std::endl;
            outStream << "[playerIIName] ";
            i = 0;
            while (i < 12 && i < C_playerIIName.getSize()) {
                outStream<<C_playerIIName[i]<<" ";
                ++i;
            }
            outStream << "0" << std::endl;
            outStream << "[playerIIKeys] "          <<  C_playerIIup << " "<< C_playerIIright << " " << C_playerIIleft << " " << C_playerIIfire << " " << C_playerIISpecialKey <<  std::endl;
            outStream << "[playerIIColor] "         <<  C_playerIIColor.r() << " "<< C_playerIIColor.g() << " " << C_playerIIColor.b() << std::endl;
            outStream << "[playerIITeamColor] "     <<  C_playerIITeamColor.r() << " "<< C_playerIITeamColor.g() << " " << C_playerIITeamColor.b() << std::endl;
            outStream << "[playerIITeamL] "         << (C_playerIIteamL ? "true" : "false") << std::endl;
            outStream << "[playerIITeamR] "         << (C_playerIIteamR ? "true" : "false") << std::endl;
            outStream << "[playerIIShip] "          <<  C_playerIIShip << std::endl;
            outStream << "[playerIIWeapon] "        <<  C_playerIIWeapon << std::endl;
            outStream << "[playerIISpecial] "       <<  C_playerIISpecial << std::endl;
            outStream << "[connectIP] "             <<  C_ip.toAnsiString() << std::endl;
            outStream << "[connectPort] "           <<  C_port.toAnsiString() << std::endl;
            outStream << "[networkTeamRed] "        << (C_networkPlayerI ? "true" : "false") << std::endl;
            outStream << "[showInfoHide] "          << (C_showInfoHide ? "true" : "false") << std::endl;
            outStream << "[showInfoSB] "            << (C_showInfoSB ? "true" : "false") << std::endl;
            outStream << "[showInfoDM] "            << (C_showInfoDM ? "true" : "false") << std::endl;
            outStream << "[showInfoTDM] "           << (C_showInfoTDM ? "true" : "false") << std::endl;
            outStream << "[showInfoCK] "            << (C_showInfoCK ? "true" : "false") << std::endl;
            outStream << "[showSelectLanguage] "    << (C_showSelectLanguage ? "true" : "false") << std::endl;
            outStream << "[showToolTips] "          << (C_showToolTips ? "true" : "false") << std::endl;
            outStream << "[languageID] "            <<  C_languageID << std::endl;
            outStream << "[highStarResolution] "    << (C_StarsHigh ? "true" : "false") << std::endl;
            outStream << "[starField] "             <<  C_StarField << std::endl;
            outStream << "[shaders] "               << (C_shaders ? "true" : "false") << std::endl;
            outStream << "[resolutionX] "           << C_resX << std::endl;
            outStream << "[resolutionY] "           << C_resY << std::endl;
            outStream << "[colorDepth] "            << C_colorDepth << std::endl;
            outStream << "[audioRandom] "           << (C_audioRandom ? "true" : "false") << std::endl;
            outStream << "[audioNextKey] "          << C_audioNextKey << std::endl;
            outStream << "[audioPreviousKey] "      << C_audioPreviousKey << std::endl;
            outStream << "[screenShotKey] "         << C_screenShotKey << std::endl;
            outStream << "[statisticsKey] "         << C_statisticsKey << std::endl;
            outStream << "[screenShotFormat] "      << C_screenShotFormat << std::endl;
            outStream << "[enabledWeapons] "        << C_EnabledWeaponsByUser << std::endl;
            outStream << "[enabledSpecials] "       << C_EnabledSpecialsByUser << std::endl;

            outStream.close();

            return true;
        }
        else {
            std::cout << "Faild to save configuration file " << C_configPath << "mars.cfg!" << std::endl;
            return false;
        }
    }

    bool load() {
        // check whether application directory in the home diretory exists, if not create it
        # ifdef __linux__
            std::string home(getenv("HOME"));
            if (*home.rbegin() != '/') home += '/';
            mkdir((home + ".marsshooter/").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        # endif

        # ifdef __WIN32__
            TCHAR szAppData[MAX_PATH];
            SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szAppData);
            std::string home(szAppData);
            CreateDirectory((home + "/.marsshooter/").c_str(), NULL);
        # endif

        # ifdef __APPLE__
            std::string home(getenv("HOME"));
            if (*home.rbegin() != '/') home += '/';
            mkdir((home + "Library/Preferences/mars/").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        # endif

        // search for config file
        if (C_configPath == "") {
            bool success(false);
            std::cout << "Searching for configuration file... " << std::flush;
            C_configPath = "./";

            # ifdef __linux__
                std::string home(getenv("HOME"));
                if (*home.rbegin() != '/') home += '/';

                C_configPath =      home + ".marsshooter/";
                if (std::ifstream((C_configPath + "mars.cfg").c_str()))
                    success = true;
            # endif

            # ifdef __WIN32__
                TCHAR szAppData[MAX_PATH];
                SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szAppData);
                std::string home(szAppData);

                if (std::ifstream((C_configPath + "mars.cfg").c_str()))
                    success = true;
                else if (std::ifstream((home + "/.marsshooter/mars.cfg").c_str())) {
                    C_configPath =      home + "/.marsshooter/";
                    success = true;
                }
                else
                    C_configPath =      home + "/.marsshooter/";
            # endif

            # ifdef __APPLE__
                std::string home(getenv("HOME"));
                if (*home.rbegin() != '/') home += '/';

                if (std::ifstream((home + "Library/Preferences/mars/mars.cfg").c_str())) {
                    C_configPath =      home + "Library/Preferences/mars/";
                    success = true;
                }
                else {
                    C_configPath =      home + "Library/Preferences/mars/";
                }
            # endif


            if (success) std::cout << "Found " << C_configPath << "mars.cfg"  << std::endl;
            else         std::cout << "Found nothing. Will create a new one." << std::endl;
        }

        // search for data files
        if (C_dataPath == "") {
            bool success(false);
            std::cout << "Searching for data files... " << std::flush;
            C_dataPath = "@DATADIR@/marsshooter/";

            # ifdef __linux__
                if (std::ifstream((C_dataPath + "locales/English.txt").c_str()))
                    success = true;
            # endif

            # ifdef __WIN32__
                if (std::ifstream((C_dataPath + "locales/English.txt").c_str()))
                    success = true;
            # endif

            # ifdef __APPLE__
                if (std::ifstream("../Resources/data/locales/English.txt")) {
                    C_dataPath = "../Resources/data/";
                    success = true;
                }
            # endif

            if (success) std::cout << "Found " << C_dataPath << std::endl;
            else {
                std::cout << "Found nothing. Aborting." << std::endl;
                return false;
            }
        }

        std::vector<sf::String> lines;
        if (file::load(C_configPath + "mars.cfg", lines)) {
            for (std::vector<sf::String>::iterator it = lines.begin(); it != lines.end(); ++it) {
                std::istringstream iss (it->toAnsiString());
                std::string inputLine;
                iss >> inputLine;
                if      (inputLine == "[soundVolume]") {
                    int value;
                    iss >> value;
                    C_soundVolume = clamp(value, 0, 100);
                }
                else if (inputLine == "[announcerVolume]") {
                    int value;
                    iss >> value;
                    C_announcerVolume = clamp(value, 0, 100);
                }
                else if (inputLine == "[musicVolume]") {
                    int value;
                    iss >> value;
                    C_musicVolume = clamp(value, 0, 100);
                }
                else if (inputLine == "[globalParticleCount]") {
                    int value;
                    iss >> value;
                    C_globalParticleCount = clamp(value, 10, 300);
                }
                else if (inputLine == "[globalParticleLifeTime]") {
                    int value;
                    iss >> value;
                    C_globalParticleLifeTime = clamp(value, 10, 300);
                }
                else if (inputLine == "[showFPS]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showFPS = true;
                    else if (value == "false")  C_showFPS = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showParticleCount]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showParticleCount = true;
                    else if (value == "false")  C_showParticleCount = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showLatency]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showLatency = true;
                    else if (value == "false")  C_showLatency = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[fullScreen]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_fullScreen = true;
                    else if (value == "false")  C_fullScreen = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[vsync]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_vsync = true;
                    else if (value == "false")  C_vsync = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[drawAIPath]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_drawAIPath = true;
                    else if (value == "false")  C_drawAIPath = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[iDumb]") {
                    int value;
                    iss >> value;
                    C_iDumb = clamp(value, 0, 100);
                }
                else if (inputLine == "[adaptiveParticleCount]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_adaptiveParticleCount = true;
                    else if (value == "false")  C_adaptiveParticleCount = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[drawBotJobs]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_drawBotJobs = true;
                    else if (value == "false")  C_drawBotJobs = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[drawZones]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_drawZones = true;
                    else if (value == "false")  C_drawZones = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[botsLeft]") {
                    int value;
                    iss >> value;
                    C_botsLeft = clamp(value, 0, 20);
                }
                else if (inputLine == "[botsRight]") {
                    int value;
                    iss >> value;
                    C_botsRight = clamp(value, 0, 20);
                }
                else if (inputLine == "[botsDeath]") {
                    int value;
                    iss >> value;
                    C_botsDeath = clamp(value, 0, 50);
                }
                else if (inputLine == "[pointLimitSB]") {
                    int value;
                    iss >> value;
                    C_pointLimitSB = clamp(value, 1, 100);
                }
                else if (inputLine == "[pointLimitCK]") {
                    int value;
                    iss >> value;
                    C_pointLimitCK = clamp(value, 1, 100);
                }
                else if (inputLine == "[pointLimitDM]") {
                    int value;
                    iss >> value;
                    C_pointLimitDM = clamp(value, 1, 100);
                }
                else if (inputLine == "[pointLimitTDM]") {
                    int value;
                    iss >> value;
                    C_pointLimitTDM = clamp(value, 1, 100);
                }
                else if (inputLine == "[powerUpRate]") {
                    int value;
                    iss >> value;
                    C_powerUpRate = clamp(value, 0, 100);
                }
                else if (inputLine == "[slowMoKickIn]") {
                    int value;
                    iss >> value;
                    C_slowMoKickIn = clamp(value, 0, 10);
                }
                else if (inputLine == "[gameSpeed]") {
                    int value;
                    iss >> value;
                    C_gameSpeed = clamp(value, 50, 200);
                }
                else if (inputLine == "[playerIName]") {
                    sf::String tmp;
                    sf::Uint32 character(0);
                    iss >> character;
                    int i(0);
                    while (character != 0 && i++ < 12) {
                        tmp.insert(tmp.getSize(), character);
                        iss >> character;
                    }
                    C_playerIName = tmp;
                }
                else if (inputLine == "[playerIIName]") {
                    sf::String tmp;
                    sf::Uint32 character(0);
                    iss >> character;
                    int i(0);
                    while (character != 0 && i++ < 12) {
                        tmp.insert(tmp.getSize(), character);
                        iss >> character;
                    }
                    C_playerIIName = tmp;
                }
                else if (inputLine == "[playerIKeys]") {
                    iss >> C_playerIup >> C_playerIright >> C_playerIleft >> C_playerIfire >> C_playerISpecialKey;
                }
                else if (inputLine == "[playerIIKeys]") {
                    iss >> C_playerIIup >> C_playerIIright >> C_playerIIleft >> C_playerIIfire >> C_playerIISpecialKey;
                }
                else if (inputLine == "[playerIColor]") {
                    float r, g, b;
                    iss >> r;
                    iss >> g;
                    iss >> b;
                    C_playerIColor = Color3f(r,g,b);
                }
                else if (inputLine == "[playerIIColor]") {
                    float r, g, b;
                    iss >> r;
                    iss >> g;
                    iss >> b;
                    C_playerIIColor = Color3f(r,g,b);
                }
                else if (inputLine == "[playerITeamColor]") {
                    float r, g, b;
                    iss >> r;
                    iss >> g;
                    iss >> b;
                    C_playerITeamColor = Color3f(r,g,b);
                }
                else if (inputLine == "[playerIITeamColor]") {
                    float r, g, b;
                    iss >> r;
                    iss >> g;
                    iss >> b;
                    C_playerIITeamColor = Color3f(r,g,b);
                }
                else if (inputLine == "[playerITeamL]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_playerIteamL = true;
                    else if (value == "false")  C_playerIteamL = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[playerITeamR]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_playerIteamR = true;
                    else if (value == "false")  C_playerIteamR = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[playerIShip]") {
                    iss >> C_playerIShip;
                }
                else if (inputLine == "[playerIIShip]") {
                    iss >> C_playerIIShip;
                }
                else if (inputLine == "[playerIWeapon]") {
                    int tmp;
                    iss >> tmp;
                    if (tmp == weapons::wInsta)
                        tmp = weapons::wAFK47;
                    C_playerIWeapon = static_cast<weapons::WeaponType>(tmp);
                }
                else if (inputLine == "[playerISpecial]") {
                    int tmp;
                    iss >> tmp;
                    C_playerISpecial = static_cast<specials::SpecialType>(tmp);
                }
                else if (inputLine == "[playerIIWeapon]") {
                    int tmp;
                    iss >> tmp;
                    if (tmp == weapons::wInsta)
                        tmp = weapons::wAFK47;
                    C_playerIIWeapon = static_cast<weapons::WeaponType>(tmp);
                }
                else if (inputLine == "[playerIISpecial]") {
                    int tmp;
                    iss >> tmp;
                    C_playerIISpecial = static_cast<specials::SpecialType>(tmp);
                }
                else if (inputLine == "[playerIITeamL]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_playerIIteamL = true;
                    else if (value == "false")  C_playerIIteamL = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[playerIITeamR]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_playerIIteamR = true;
                    else if (value == "false")  C_playerIIteamR = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[connectIP]") {
                    std::string tmp;
                    iss >> tmp;
                    C_ip = sf::String(tmp);
                }
                else if (inputLine == "[connectPort]") {
                    std::string tmp;
                    iss >> tmp;
                    C_port = sf::String(tmp);
                }
                else if (inputLine == "[languageID]") {
                    int value;
                    iss >> value;
                    C_languageID = value;
                }
                else if (inputLine == "[networkTeamRed]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_networkPlayerI = true;
                    else if (value == "false")  C_networkPlayerI = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showInfoHide]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showInfoHide = true;
                    else if (value == "false")  C_showInfoHide = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showInfoSB]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showInfoSB = true;
                    else if (value == "false")  C_showInfoSB = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showInfoDM]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showInfoDM = true;
                    else if (value == "false")  C_showInfoDM = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showInfoTDM]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showInfoTDM = true;
                    else if (value == "false")  C_showInfoTDM = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showInfoCK]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showInfoCK = true;
                    else if (value == "false")  C_showInfoCK = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[showToolTips]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showToolTips = true;
                    else if (value == "false")  C_showToolTips = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[highStarResolution]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_StarsHigh = true;
                    else if (value == "false")  C_StarsHigh = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[starField]") {
                    int value;
                    iss >> value;
                    C_StarField = value;
                }
                else if (inputLine == "[showSelectLanguage]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_showSelectLanguage = true;
                    else if (value == "false")  C_showSelectLanguage = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[shaders]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_shaders = true;
                    else if (value == "false")  C_shaders = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[resolutionX]") {
                    int value;
                    iss >> value;
                    C_resX = value;
                }
                else if (inputLine == "[resolutionY]") {
                    int value;
                    iss >> value;
                    C_resY = value;
                }
                else if (inputLine == "[colorDepth]") {
                    int value;
                    iss >> value;
                    C_colorDepth = value;
                }
                else if (inputLine == "[audioRandom]") {
                    std::string value;
                    iss >> value;
                    if (value == "true")        C_audioRandom = true;
                    else if (value == "false")  C_audioRandom = false;
                    else std::cout << value << " is a bad value for " << inputLine << ". Use true or false instead.\n";
                }
                else if (inputLine == "[audioNextKey]") {
                    iss >> C_audioNextKey;
                }
                else if (inputLine == "[audioPreviousKey]") {
                    iss >> C_audioPreviousKey;
                }
                else if (inputLine == "[screenShotKey]") {
                    iss >> C_screenShotKey;
                }
                else if (inputLine == "[statisticsKey]") {
                    iss >> C_statisticsKey;
                }
                else if (inputLine == "[screenShotFormat]") {
                    iss >> C_screenShotFormat;
                }
                else if (inputLine == "[enabledWeapons]") {
                    iss >> C_EnabledWeaponsByUser;
                    C_EnabledWeapons = C_EnabledWeaponsByUser;
                }
                else if (inputLine == "[enabledSpecials]") {
                    iss >> C_EnabledSpecialsByUser;
                    C_EnabledSpecials = C_EnabledSpecialsByUser;
                }
                else
                    std::cout << inputLine << " is a bad option in " << C_configPath << "mars.cfg!\n";
            }

        }
        else {
            if (save())
                std::cout << "Created " << C_configPath << "mars.cfg, using default settings.\n";
        }
        return true;
    }
}
