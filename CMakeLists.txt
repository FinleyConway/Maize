cmake_minimum_required(VERSION 3.26)
project(Maize)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR Maize/vendors/SFML-2.6.0/lib/cmake/SFML)

find_package(SFML 2.6.0 COMPONENTS system window graphics audio network REQUIRED)

add_subdirectory(Maize)
add_subdirectory(MaizeOil)

target_link_libraries(Maize PUBLIC sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
