cmake_minimum_required(VERSION 3.29)
project(frogy)

set(CMAKE_CXX_STANDARD 14)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})



add_executable(frogy
    main.c)
target_link_libraries(frogy ${CURSES_LIBRARIES})
