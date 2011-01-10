/* locales.hpp

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

# ifndef LOCALES_HPP_INCLUDED
# define LOCALES_HPP_INCLUDED

# include <SFML/System.hpp>
# include <vector>

/// A namespace which handles translations for MARS.

namespace locales {
    /// The different texts included in MARS.
    enum LocaleType {FontName, StartNetworkGame, JoinNetworkGame, Options, About, Quit, Continue, RestartGame,
                     HideMenu, QuitCurrentGame, Start, Cancel, Info, Close, License, Ok, ShowAgainButton, LeftTeam,
                     RightTeam, BotsLeft, BotsRight, Bots, Fraglimit, InfoSB, ShortDescriptionSB, InfoDM,
                     ShortDescriptionDM, InfoTDM, ShortDescriptionTDM, InfoCK, ShortDescriptionCK, DebuggingInformation,
                     BotsOrientation, Zones, AIPaths, GameInformation, FramesPerSecond, ParticleCount,
                     ParticleCountSlider, ParticleLifetime, GameSettings, Fullscreen, VerticalSynchronisation, MusicVolume,
                     SoundVolume, AnnouncerVolume, Name, Accelerate, TurnLeft, TurnRight, Weapon, ShipName, PlayerColor,
                     TeamColor, SPACESPACE, KeyEditHover, TextEditHover, AboutText, LicenseText, QuitText,
                     QuitCurrentGameText, StartNetworkText, JoinNetworkText, HideMenuText, Add, BackSlash, Comma, Dash,
                     Delete, Divide, Down, End, Equal, Home, Insert, LeftAlt, LeftBracket, LeftControl, Left, LeftShift,
                     LeftSuper, Menu, Multiply, PageDown, PageUp, Pause, Period, Quote, RightAlt, RightBracket,
                     RightControl, Return,Right, RightShift, RightSuper, SemiColon, Slash, Space, Subtract, Tilde, Tab,
                     Up, Display, Audio, WindowSettings, StartTutorial,Next, Tut01, TutText01, Tut02, TutText02, Tut03,
                     TutText03, Tut04, TutText04, Tut05, TutText05, Tut06, TutText06, Tut07, TutText07, Tut08, TutText08,
                     TutText08b, Tut09, TutText09, Tut10, TutText10, TutText10b, TutText10c, Tut11, TutText11, Tut12,
                     TutText12, Tut13, TutText13, Tut14, TutText14, Tut15, TutText15, Tut16, TutText16, Tut17, TutText17,
                     Tut18, TutText18, Tut19, TutText19, Tut20, TutText20, Interface, StarsHigh, Language, ScreenShotKey,
                     Points, Frags, TeamKills, Suicides, Deaths, Reputation, Total, Statistics, Pointlimit, Shaders,
                     ShaderError, Gameplay, PowerUpRate, iDumb, SlowMoKickIn, SlowMoOff, Credits, SpecialThanks, CreditText,
                     Resolution, ColorDepth, Special, StartLocalGame,
                     COUNT///< COUNT contains the amount of entries in the enum. Keep it the last!
    };

    /// Loads the current locale, accordind to settings::C_language.
    void                          load();

    /// Returns a list of all files in data/locales.
    std::vector<sf::String> const getLanguages();

    /// Returns a string accordind to the current locale.
    sf::String*                   getLocale(LocaleType);
}

# endif // LOCALES_HPP_INCLUDED

