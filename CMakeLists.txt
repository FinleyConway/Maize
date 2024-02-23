cmake_minimum_required(VERSION 3.26)
project(Maize)

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# warnings
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
ENDIF ()

# Add subdirectories for your projects
add_subdirectory(Maize)
add_subdirectory(MaizeOil)