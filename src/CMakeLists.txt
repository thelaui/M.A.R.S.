file(GLOB MARS_SOURCES */*.cpp)
file(GLOB MARS_HEADERS ${MARS_SOURCE_DIR}/include/*/*.hpp)

add_executable(mars main.cpp ${MARS_SOURCES} ${MARS_HEADERS})

target_link_libraries (mars sfml-system sfml-window sfml-graphics sfml-audio)
