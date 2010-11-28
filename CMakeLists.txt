PROJECT(MARS)
cmake_minimum_required(VERSION 2.8)

set(EXECUTABLE_OUTPUT_PATH ${MARS_SOURCE_DIR})

include_directories(${MARS_SOURCE_DIR}/include)

add_subdirectory(src)


