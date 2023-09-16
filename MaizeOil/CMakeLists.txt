project(MaizeOil)

set(RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets)
set(SAVES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Saves)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${RESOURCES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${SAVES_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) #temp

add_executable(MaizeOil src/MaizeOilApp.cpp
        src/Sandbox.h
        src/CartesianGrid.cpp
        src/CartesianGrid.h
        src/TilemapEditor/Tileset/Tileset.cpp
        src/TilemapEditor/Tileset/Tileset.h
        src/TilemapEditor/Tileset/TilesetWindowTab.cpp
        src/TilemapEditor/Tileset/TilesetWindowTab.h
        src/TilemapEditor/Tilemap/TilemapWindowTab.cpp
        src/TilemapEditor/Tilemap/TilemapWindowTab.h
        src/TilemapEditor/Tilemap/TilemapLayer.cpp
        src/TilemapEditor/Tilemap/TilemapLayer.h
        src/TilemapEditor/TilemapEditorWindow.cpp
        src/TilemapEditor/TilemapEditorWindow.h
        src/TilemapEditor/Tileset/Tile.h
        src/TilemapEditor/Tileset/TilesetSerializer.cpp
        src/TilemapEditor/Tileset/TilesetSerializer.h
        src/TilemapEditor/Tilemap/TilemapSerializer.cpp
        src/TilemapEditor/Tilemap/TilemapSerializer.h)

target_link_libraries(MaizeOil PRIVATE Maize)

target_compile_definitions(MaizeOil PRIVATE RESOURCES_DIR="${RESOURCES_DIR}")
target_compile_definitions(MaizeOil PRIVATE ASSETS_DIR="${ASSETS_DIR}")
target_compile_definitions(MaizeOil PRIVATE SAVES_DIR="${SAVES_DIR}")