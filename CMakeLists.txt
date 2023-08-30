cmake_minimum_required(VERSION 3.26)
project(Maize)

include(FetchContent)

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Disable building shared libraries, use static linking
set(BUILD_SHARED_LIBS OFF)

# Fetch SFML library
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
)

# Fetch ImGui library
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG docking
)

# Fetch ImGui-SFML library
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG 2.6.x
)

# Fetch Nlohmann Json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

# Set SFML_DIR to point to sfml_BINARY_DIR
set(SFML_DIR "${sfml_BINARY_DIR}")

FetchContent_MakeAvailable(SFML)
FetchContent_MakeAvailable(imgui)

# Set options
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" ON)

FetchContent_MakeAvailable(imgui-sfml)
FetchContent_MakeAvailable(nlohmann_json)

# Add subdirectories for your projects
add_subdirectory(Maize)
add_subdirectory(MaizeOil)

# Link libraries to the Maize target
target_link_libraries(Maize
        PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        ImGui-SFML::ImGui-SFML
        nlohmann_json::nlohmann_json
)
