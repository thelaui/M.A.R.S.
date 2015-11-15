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

# include "Locales/Locale.hpp"

# include <vector>
# include <libintl.h>
# include <SFML/System/String.hpp>

/// A macro to make i18n more readable and aid in tagging strings for translation
#define _(str) locales::string2sfstring(std::string(locales::translate(str)))


/// A namespace which handles translations for MARS.

namespace locales {

	char const * translate(char        const *) __attribute__ ((format_arg (1)));
	char const * translate(const std::string &);

	/** Wrappers for sprintf. Format 'format' with the variables var1...
	 *
	 * In order to keep the strings' encoding intact, translate strings with: gettext("Translate %s")
	 * Do not use the _("Translate %s") macro!
	 *
	 * Settings members need to be fetched with:
	 *
	 * reinterpret_cast<const char*>(settings_sfstring.toUtf8().c_str()),
	 *
	 * Writing a wrapper for this didn't work because of temporaries.
	 */
	sf::String* format_string(const char* format, const char* var1);
	sf::String* format_string(const char* format, const char* var1, const char* var2);
	sf::String* format_string(const char* format, const char* var1, const char* var2, const char* var3);

	/// Converts a std::string to an sf::String while keeping the encoding intact.
	sf::String* string2sfstring(const std::string& string);

	 /// Loads the current locale, accordind to settings::C_languageID.
    bool                       load();

    /// Returns a list of all files in data/locales.
    std::vector<Locale> const& getLocales();

    Locale const&              getCurrentLocale();
	 void setCurrentLocale();
}

# endif // LOCALES_HPP_INCLUDED

