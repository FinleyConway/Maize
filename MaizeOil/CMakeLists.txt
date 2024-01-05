project(MaizeOil)

set(ASSETS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Assets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${ASSETS_DIR} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(MaizeOil src/MaizeOilApp.cpp
        src/LevelEditor.h
        src/Playground.h
)

target_link_libraries(MaizeOil PRIVATE Maize)

target_compile_definitions(MaizeOil PRIVATE RESOURCES_DIR="${RESOURCES_DIR}")
target_compile_definitions(MaizeOil PRIVATE ASSETS_DIR="${ASSETS_DIR}")
target_compile_definitions(MaizeOil PRIVATE SAVES_DIR="${SAVES_DIR}")