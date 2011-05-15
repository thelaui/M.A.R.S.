/* randomizer.cpp

This code is taken from a previous version of SFML, since this namespace
has been dropped. So credits go to Laurent Gomila (laurent.gom@gmail.com)

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

#include "System/randomizer.hpp"

#include <cstdlib>
#include <ctime>

namespace {

    unsigned int InitializeSeed() {
        unsigned int seed = static_cast<unsigned int>(std::time(NULL));
        std::srand(seed);
        return seed;
    }

    unsigned int globalSeed = InitializeSeed();
}

void randomizer::setSeed(unsigned int seed) {
    std::srand(seed);
    globalSeed = seed;
}

unsigned int randomizer::getSeed() {
    return globalSeed;
}

float randomizer::random(float begin, float end) {
    return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}

int randomizer::random(int begin, int end) {
    return std::rand() % (end - begin + 1) + begin;
}
