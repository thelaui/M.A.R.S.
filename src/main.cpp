/* main.cpp

Copyright (c) 2010 by Felix Lauer and Simon Schneegans

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

# ifdef __APPLE__
# include "CoreFoundation/CoreFoundation.h"
# endif

# include "System/window.hpp"
# include "Games/games.hpp"

int main() {

#if defined __APPLE__
	static char path[1024]="";

	if(strlen(path)==0) {
		// Determining the Resources path inside the .app bundle
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL( mainBundle ); 
		if( !CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, sizeof(path)) )
			fprintf(stderr, "Unable to determine data path");
		CFRelease(resourcesURL);
		chdir(path);
	}
#endif
	
	
    window::open();
    games:: start(games::gMenu);
    window::mainLoop();

    return 0;
}
