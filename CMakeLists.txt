cmake_minimum_required(VERSION 3.26)
project(Maize)

include(FetchContent)

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
ENDIF ()

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
        GIT_TAG v1.89.9-docking
)

# Fetch ImGui-SFML library
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v2.6
)

# Fetch Nlohmann Json
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

# Fetch Portable File Dialogs
FetchContent_Declare(
        pfd
        GIT_REPOSITORY https://github.com/samhocevar/portable-file-dialogs.git
        GIT_TAG 0.1.0
)

# Fetch my ecs project
FetchContent_Declare(
        ecs
        GIT_REPOSITORY https://github.com/FinleyConway/EntityComponentSystem.git
        GIT_TAG master
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
FetchContent_MakeAvailable(pfd)
FetchContent_MakeAvailable(ecs)

# Add subdirectories for your projects
add_subdirectory(Maize)
add_subdirectory(MaizeOil)

target_include_directories(Maize PUBLIC ${pfd_SOURCE_DIR})
target_include_directories(Maize PUBLIC ${ecs_SOURCE_DIR})

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
