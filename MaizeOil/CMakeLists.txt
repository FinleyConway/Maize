project(MaizeOil)

set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(SAVES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Saves)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${RESOURCES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${SAVES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) #temp

add_executable(MaizeOil src/MaizeOilApp.cpp
        src/LevelEditor.h
        ../Maize/src/Maize/Tilemap/CartesianGrid.cpp
        ../Maize/src/Maize/Tilemap/CartesianGrid.h
        src/TilemapEditor/TilesetWindowTab.cpp
        src/TilemapEditor/TilesetWindowTab.h
        src/TilemapEditor/TilemapWindowTab.cpp
        src/TilemapEditor/TilemapWindowTab.h
        ../Maize/src/Maize/Tilemap/TilemapLayer.cpp
        ../Maize/src/Maize/Tilemap/TilemapLayer.h
        src/TilemapEditor/TilemapEditorWindow.h
        ../Maize/src/Maize/Tilemap/TilesetSerializer.cpp
        ../Maize/src/Maize/Tilemap/TilesetSerializer.h
        ../Maize/src/Maize/Tilemap/TilemapSerializer.cpp
        ../Maize/src/Maize/Tilemap/TilemapSerializer.h)

target_link_libraries(MaizeOil PRIVATE Maize)

target_compile_definitions(MaizeOil PRIVATE RESOURCES_DIR="${RESOURCES_DIR}")
target_compile_definitions(MaizeOil PRIVATE ASSETS_DIR="${ASSETS_DIR}")
target_compile_definitions(MaizeOil PRIVATE SAVES_DIR="${SAVES_DIR}")