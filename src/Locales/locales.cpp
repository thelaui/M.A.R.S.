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

#ifdef _WIN32
# include <windows.h>
# include <cstdlib>
#endif

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

	sf::String* format_string(const char* format, const char* var1) {
		char buffer [2 * (strlen(format) + strlen(var1))];
		sprintf(buffer, format, var1);
		return string2sfstring(std::string(buffer));
	}


	sf::String* format_string(const char* format, const char* var1, const char* var2) {
		char buffer [2 * (strlen(format) + strlen(var1) + strlen(var2))];
		sprintf(buffer, format, var1, var2);
		return string2sfstring(std::string(buffer));
	}

	sf::String* format_string(const char* format, const char* var1, const char* var2, const char* var3) {
		char buffer [2 * (strlen(format) + strlen(var1) + strlen(var2) + strlen(var3))];
		sprintf(buffer, format, var1, var2, var3);
		return string2sfstring(std::string(buffer));
	}

	sf::String* string2sfstring(const std::string& string) {
		sf::String result = sf::String::fromUtf8(string.begin(), string.end());
		return new sf::String(result);
	}

	 namespace {
		  std::vector<Locale>     locales_;
	 } // namespace

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
								newLocale.iso_ = arg;
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

	 /**
	  * Set the locale to the given string.
	  * Code inspired by wesnoth.org
	  */
	 void setCurrentLocale() {
		  char const * const dom = "marsshooter";

		  char const * const localedir = (settings::C_dataPath + std::string("locale")).c_str();
		  std::cout << "localedir " << localedir;

		  char const * const loc =  getCurrentLocale().iso_.toAnsiString().c_str();
		  std::cout << "localedir: " << localedir << " locale: " << loc << "\n";

#ifndef _WIN32
#ifndef __AMIGAOS4__
#ifndef __APPLE__
		  unsetenv ("LANGUAGE"); // avoid problems with this variable
#endif
#endif
#endif
#ifdef __BEOS__
		  setenv ("LANG",   loc, 1);
		  setenv ("LC_ALL", loc, 1);
#endif
#ifdef __APPLE__
		  setenv ("LANGUAGE", loc, 1);
		  setenv ("LANG",     loc, 1);
		  setenv ("LC_ALL",   loc, 1);
#endif
#ifdef _WIN32
#ifdef __MINGW32__
// Mingw doesn't define _putenv_s()
extern int _putenv_s(const char*, const char*);
#endif
		  _putenv_s("LANG", loc);
#endif

#ifdef __linux__
		  setenv("LANGUAGE", loc, 1);
#endif
		  // set locale according to the env. variables
		  SETLOCALE(LC_ALL, "");
		  SETLOCALE(LC_NUMERIC, "C");
		  SETLOCALE(LC_MESSAGES, "");

		  textdomain(dom);
		  bindtextdomain(dom, localedir);
		  bind_textdomain_codeset(dom, "UTF-8");
	 }

} // namespace locales
