/* locales.cpp

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

# include "Locales/locales.hpp"

#ifdef __FreeBSD__
#  include <clocale>
#endif
#  include <cstring>

# include "System/settings.hpp"
# include "System/generateName.hpp"
# include "Media/file.hpp"
# include "Locales/Locale.hpp"

# include <sstream>
# include <iostream>

#ifdef __APPLE__
# if LIBINTL_VERSION >= 0x001201
// for older libintl versions, setlocale is just fine
#  define SETLOCALE libintl_setlocale
# endif // LIBINTL_VERSION
#else // __APPLE__
# if defined _WIN32
#  define SETLOCALE setlocale
# else
#  define SETLOCALE std::setlocale
# endif
#endif

extern int _nl_msg_cat_cntr;

namespace locales {
	/**
	 * Translate a string with gettext
	 */
	char const * translate(char const * const str) {
		return gettext(str);
	}
	char const * translate(const std::string & str) {
		return gettext(str.c_str());
	}

	sf::String* format_string(const sf::String* format, const char* var1) {
		const std::string f = format->toAnsiString();
		char buffer [f.size() + strlen(var1)];
		sprintf(buffer, f.c_str(), var1);
		return new sf::String(buffer);
	}

	sf::String* format_string(const sf::String* format, const char* var1, const char* var2) {
		const std::string f = format->toAnsiString();
		char buffer [f.size() + strlen(var1) + strlen(var2)];
		sprintf(buffer, f.c_str(), var1, var2);
		return new sf::String(buffer);
	}

	sf::String* format_string(const sf::String* format, const char* var1, const char* var2, const char* var3) {
		const std::string f = format->toAnsiString();
		char buffer [f.size() + strlen(var1) + strlen(var2) + strlen(var3)];
		sprintf(buffer, f.c_str(), var1, var2, var3);
		return new sf::String(buffer);
	}

    namespace {
	 // NOCOM
#define COUNT 1
        std::vector<Locale>     locales_;
        std::vector<sf::String> localeStrings_(COUNT, "Error");
    }

    bool load() {
        std::vector<sf::String> lines;
        if (file::load(settings::C_dataPath + "locales/locales.conf", lines)) {
            locales_.clear();
            Locale newLocale;
            bool first(true);
            for (std::vector<sf::String>::iterator it = lines.begin(); it != lines.end(); ++it) {
                if ((*it).toAnsiString()[0] == '[') {
                    if (!first) {
                        locales_.push_back(newLocale);
                        newLocale = Locale();
                    }
                    newLocale.name_ = *it;
                    newLocale.name_.erase(0, 1);
                    newLocale.name_.erase(newLocale.name_.getSize()-1, 1);

                    first = false;
                }
                else {
                    std::stringstream sstr(std::string((*it).toAnsiString()));
                    std::string flag;
                    sstr >> flag;

                    sf::String arg(*it);
                    arg.erase(0, flag.size()+1);

						  if (flag == "iso:")
								newLocale.iso_ = arg.toAnsiString().c_str();
                    else if (flag == "font:")
                        newLocale.font_ = arg;
                    else if (flag == "author:")
                        newLocale.author_ = "By " + arg;
                    else if (flag == "direction:") {
                        if (arg == "RTL") newLocale.LTR_ = false;
                        else              newLocale.LTR_ = true;
                    }
                    else
                        std::cout << "Unrecognized flag \"" << flag << "\" in " << settings::C_dataPath + "locales/locales.conf !" << std::endl;
                }
            }

            if (!first)
                locales_.push_back(newLocale);

				/* NOCOM do something with gettext system here
            if (settings::C_languageID < locales_.size()) {

					 if (!load (locales_[settings::C_languageID].iso_)) {
						  std::cout << "Failed to load locale/" << locales_[settings::C_languageID].iso_.toAnsiString() << "! Falling back to English..." << std::endl;
                    settings::C_languageID = 0;
                }


            }
            else {
                std::cout << "Specified language in mars.conf doesn't match any in locales.conf! Falling back to English..." << std::endl;
                settings::C_languageID = 0;
            }
				*/

            return true;
        }
        else {
            std::cout << "Not found! Aborting..." << std::endl;
            return false;
        }
    }

    std::vector<Locale>const& getLocales() {
        return locales_;
    }

    Locale const& getCurrentLocale() {
        return locales_[settings::C_languageID];
    }

	 bool setCurrentLocale() {
		  // NOCOM bool success = load(settings::C_dataPath + "locales/"+locales_[settings::C_languageID].iso_);

		  char const * const dom = "marsshooter";
		  char const * const ldir = (settings::C_dataPath + std::string("locale")).c_str();
			std::cout << "localedir " << ldir;
		  setlocale (LC_ALL, "");
		  bindtextdomain(dom, ldir);
		  bind_textdomain_codeset(dom, "UTF-8");
		  textdomain(dom);
		  return true;
    }

	 const char* get_string(const sf::String& string) {
		 return string.toAnsiString(std::locale(locales::getCurrentLocale().iso_)).c_str();
	 }
}


