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

///A macro to make i18n more readable and aid in tagging strings for translation
#define _(str) new sf::String(locales::translate(str))

/// A namespace which handles translations for MARS.

namespace locales {

	char const * translate(char        const *) __attribute__ ((format_arg (1)));
	char const * translate(const std::string &);

	/// Wrapper for sprintf. Formats 'format' with the variables var1...
	sf::String* format_string(const sf::String* format, const char* var1);
	sf::String* format_string(const sf::String* format, const char* var1, const char* var2);
	sf::String* format_string(const sf::String* format, const char* var1, const char* var2, const char* var3);

    /// Loa ds the current locale, accordind to settings::C_languageID.
    bool                       load();

    /// Returns a list of all files in data/locales.
    std::vector<Locale> const& getLocales();

    Locale const&              getCurrentLocale();
    void                       setCurrentLocale();
}

# endif // LOCALES_HPP_INCLUDED

