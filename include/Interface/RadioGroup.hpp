/* RadioGroup.hpp

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

# ifndef RADIOGROUP_HPP_INCLUDED
# define RADIOGROUP_HPP_INCLUDED

# include "Interface/UiElement.hpp"
# include "Interface/RadioButton.hpp"

# include <vector>

class RadioButton;

class RadioGroup: public UiElement {
        public:
            RadioGroup();
            ~RadioGroup();

            void mouseMoved(Vector2f const& position);
            void mouseLeft(bool down);

            void draw() const;

            void allOff();

            void addRadioButton(RadioButton* toBeAdded);

        private:
            std::vector<RadioButton*> radioButtons_;
};

# endif

