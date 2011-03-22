
#####################################################################
# Actually compile the executable
#####################################################################

# determine source and header files
file(GLOB MARS_SOURCES */*.cpp)
file(GLOB MARS_HEADERS ${MARS_SOURCE_DIR}/include/*/*.hpp ${MARS_SOURCE_DIR}/include/*.hpp)
file(GLOB MARS_RESOURCES ${MARS_SOURCE_DIR}/mars.cfg)

# create executable
if(APPLE)
	add_executable(
		mars MACOSX_BUNDLE
			main.cpp
			${MARS_SOURCES}
			${MARS_HEADERS}
			${MARS_RESOURCES}
	)

	# Mac OS X bundle specific settings
	set(MACOSX_BUNDLE true)
	set(MACOSX_BUNDLE_BUNDLE_NAME mars)
	set(MACOSX_BUNDLE_INFO_STRING "M.A.R.S. ${mars_VERSION}")
	set(MACOSX_BUNDLE_LONG_VERSION_STRING "${mars_VERSION}")
	set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${mars_VERSION}")
	set(MACOSX_BUNDLE_BUNDLE_VERSION "${mars_VERSION}")
  set(MACOSX_BUNDLE_ICON_FILE ${MARS_SOURCE_DIR}/resources/icon.icns)

	# Let gcc 4.0 compile the executable even on Mac OS X 10.6
	# so that the executable also runs on Mac OS X 10.4
	# only valid for xcode not for command line version of gcc
	set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "4.0")
  
  # paths for data files in bundle
  set_source_files_properties(
    ${MARS_RESOURCES}
    PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
  )
else(APPLE)
	add_executable(
		mars
			main.cpp 
			${MARS_SOURCES}
			${MARS_HEADERS}
	)
endif(APPLE)

# link SFML and other libraries
target_link_libraries(
	mars
		${FOUNDATION_LIBRARY}
		${SFML_LIBRARIES}
		${OPENGL_LIBRARIES}
		${XRANDR_LIBRARY}
		${FRIBIDI_LIBRARY}
		${TAG_LIBRARY}
)

#####################################################################
# Installation
#####################################################################

if(UNIX)
	install(
		TARGETS 
			mars
		RUNTIME DESTINATION
			${CMAKE_INSTALL_PREFIX}/games
	)
	install(
		FILES
			${MARS_SOURCE_DIR}/credits.txt
			${MARS_SOURCE_DIR}/license.txt
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/doc/marsshooter
	)
	install(
		FILES
			${MARS_SOURCE_DIR}/data/botnames.txt
			${MARS_SOURCE_DIR}/data/shipnames.txt
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/games/marsshooter
	)
	install(
		DIRECTORY 
			${CMAKE_SOURCE_DIR}/data/audio
			${CMAKE_SOURCE_DIR}/data/fonts
			${CMAKE_SOURCE_DIR}/data/locales
			${CMAKE_SOURCE_DIR}/data/shaders
			${CMAKE_SOURCE_DIR}/data/tex
		DESTINATION 
			${CMAKE_INSTALL_PREFIX}/share/games/marsshooter
		PATTERN 
			".svn" EXCLUDE
	)
	# icons
	install(
		FILES
			${MARS_SOURCE_DIR}/resources/mars.png
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/512x512/apps
	)
	install(
		FILES
			${MARS_SOURCE_DIR}/resources/mars.svg
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/scalable/apps
	)
	install(
		FILES
			${MARS_SOURCE_DIR}/resources/mars.xpm
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/pixmaps
	)
	# desktop file
	install(
		FILES
			${MARS_SOURCE_DIR}/resources/mars.desktop
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/applications
	)
	install(
		FILES
			${MARS_SOURCE_DIR}/resources/mars
		DESTINATION
			${CMAKE_INSTALL_PREFIX}/share/menu
	)

else(UNIX)
	# executable
	install(
		TARGETS mars
			RUNTIME DESTINATION .
			BUNDLE DESTINATION .
	)

	# install license and credits as well
	install(
		FILES
			${MARS_SOURCE_DIR}/credits.txt
			${MARS_SOURCE_DIR}/license.txt
		DESTINATION
			.
	)
endif(UNIX)

if(APPLE)
	# install data directory
	install(
		DIRECTORY ${CMAKE_SOURCE_DIR}/data
			DESTINATION mars.app/Contents/Resources
			FILES_MATCHING
				PATTERN "*.*"
	)
	
	# copy sndfile framework into app bundle for Mac OS X
	install(
		DIRECTORY /Library/Frameworks/sndfile.framework
			DESTINATION mars.app/Contents/Frameworks
		)

	# fixup bundle, copy dynamic libraries into app bundle
	set(APPS "\${CMAKE_INSTALL_PREFIX}/mars.app")  # paths to executables
	set(DIRS "${SFML_INCLUDE_DIR}/../lib")   # directories to search for prerequisites
	INSTALL(CODE "
  	include(BundleUtilities)
   	fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   	")
endif(APPLE)
