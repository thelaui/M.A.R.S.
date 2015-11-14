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
				setCurrentLocale();

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
		  setlocale(LC_ALL, "");
		  //setlocale(LC_NUMERIC, "C");
		  //SETLOCALE(LC_MESSAGES, "de_DE.utf-8");
/*
		  std::pair<std::string, std::string> map_data[] = {
				std::make_pair("de", "de,de_DE,de_AT,de_CH,de_LI,de_LU,de_BE"),
			  std::make_pair("gd", "gd,gd_GB,gd_CA")
		  };

		  std::map<std::string, std::string> kAlternatives(map_data,
				map_data + sizeof map_data / sizeof map_data[0]);


		  //const std::map<std::string, std::string> kAlternatives;
		  //kAlternatives.insert("ar", "ar,ar_AR,ar_AE,ar_BH,ar_DZ,ar_EG,ar_IN,ar_IQ,ar_JO,ar_KW,ar_LB,ar_LY,ar_MA,ar_OM,ar_QA,ar_SA,ar_SD,ar_SY,ar_TN,ar_YE");
		  //kAlternatives.insert("ast", "ast,ast_ES");
		  //kAlternatives.insert("ca", "ca,ca_ES,ca_ES@valencia,ca_FR,ca_IT");
		  //kAlternatives.insert("cs", "cs,cs_CZ");
		  //kAlternatives.insert("da", "da,da_DK");
		  //kAlternatives.insert(std::pair<std::string, std::string>(std::string("de"), std::string("de,de_DE,de_AT,de_CH,de_LI,de_LU,de_BE")));
		  //kAlternatives.insert("el", "el,el_GR,el_CY");
		  //kAlternatives.insert("en", "en,en_US,en_GB,en_AU,en_CA,en_AG,en_BW,en_DK,en_HK,en_IE,en_IN,en_NG,en_NZ,en_PH,en_SG,en_ZA,en_ZW");
		  //kAlternatives.insert("en_AU", "en_AU,en,en_US,en_GB");
		  //kAlternatives.insert("en_CA", "en_CA,en,en_US,en_GB");
		  //kAlternatives.insert("en_GB", "en_GB,en,en_US");
		  //kAlternatives.insert("eo", "eo,eo_XX");
		  //kAlternatives.insert("es", "es,es_ES,es_MX,es_US");

		  std::string lang;
		  std::string env_locale;
		  std::string locale;

		  env_locale = std::string();
		  // Start init_locale - can this go?
	  #ifdef _WIN32
		  locale = "English";
		  SETLOCALE(LC_ALL, "English");
	  #else
		  // first, save environment variable
		  lang = getenv("LANG");
		  if (!lang.empty()) {
			  env_locale = lang;
		  }
		  if (env_locale.empty()) {
			  lang = getenv("LANGUAGE");
			  if (!lang.empty()) {
				  env_locale = lang;
			  }
		  }
		  locale = "C";
		  SETLOCALE(LC_ALL, "C");
		  SETLOCALE(LC_MESSAGES, "");
	  #endif
		  // End init_locale - can this go?
		  lang = getCurrentLocale().iso_.toAnsiString();

		  std::cout << "selected language: " << (lang.empty()?"(system language)":lang.c_str()) << "\n";

	  #ifndef _WIN32
	  #ifndef __AMIGAOS4__
	  #ifndef __APPLE__
		  unsetenv ("LANGUAGE"); // avoid problems with this variable
	  #endif
	  #endif
	  #endif

		  std::string alt_str;
		  if (lang.empty()) {
			  // reload system language, if selected
			  lang = env_locale;
			  alt_str = env_locale;
		  } else {
			  alt_str = lang;
			  // otherwise, try alternatives.
			  if (kAlternatives.count(lang.c_str())) {
				  alt_str = kAlternatives.at(lang.c_str());
			  }
		  }
		  alt_str += ",";

		  // Somehow setlocale doesn't behave same on
		  // some systems.
	  #ifdef __BEOS__
		  setenv ("LANG",   lang.c_str(), 1);
		  setenv ("LC_ALL", lang.c_str(), 1);
		  locale = lang;
	  #endif
	  #ifdef __APPLE__
		  setenv ("LANGUAGE", lang.c_str(), 1);
		  setenv ("LANG",     lang.c_str(), 1);
		  setenv ("LC_ALL",   lang.c_str(), 1);
		  locale = lang;
	  #endif
	  #ifdef _WIN32
		  _putenv_s("LANG", lang.c_str());
		  locale = lang;
	  #endif

	  #ifdef __linux__
		  char * res = NULL;
		  char const * encoding[] = {"", ".utf-8", "@euro", ".UTF-8"};
		  std::size_t found = alt_str.find(',', 0);
		  bool leave_while = false;
		  // try every possible combination of alternative and encoding
		  while (found != std::string::npos) {
			  std::string base_locale = alt_str.substr(0, int(found));
			  alt_str = alt_str.erase(0, int(found) + 1);

			  for (int j = 0; j < 4; ++j) {
				  std::string try_locale = base_locale + encoding[j];
				  res = SETLOCALE(LC_MESSAGES, try_locale.c_str());
				  if (res) {
					  locale = try_locale;
					  std::cout << "using locale " << try_locale.c_str() << "\n";
					  leave_while = true;
					  break;
				  } else {
					  //std::cout << "locale is not working: \n" << try_locale.c_str() << "\n";
				  }
			  }
			  if (leave_while) break;

			  found = alt_str.find(',', 0);
		  }
		  if (leave_while) {
			  setenv("LANG",     locale.c_str(), 1);
			  setenv("LANGUAGE", locale.c_str(), 1);
		  } else {
			  std::cout <<
				 "No corresponding locale found - trying to set it via LANGUAGE="
					<< lang.c_str() << ", LANG=" << lang.c_str() << "\n";
			  setenv("LANGUAGE", lang.c_str(), 1);
			  setenv("LANG",     lang.c_str(), 1);
			  SETLOCALE(LC_MESSAGES, "");    // set locale according to the env. variables
														// --> see  $ man 3 setlocale
			  // assume that it worked
			  // maybe, do another check with the return value (?)
			  locale = lang;
		  }

		  // Finally make changes known.
		  ++_nl_msg_cat_cntr;
	  #endif


		  SETLOCALE(LC_ALL, locale.c_str()); //  call to libintl
		  SETLOCALE(LC_MESSAGES, locale.c_str());
		  */

		  textdomain(dom);
		  bindtextdomain(dom, localedir);
		  bind_textdomain_codeset(dom, "UTF-8");

		  //std::cout << "locale: " << locale << " lang: " << lang << " env_locale: " << env_locale << "\n";
    }
} // namespace locales
