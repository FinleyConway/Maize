project(MaizeOil)

add_executable(MaizeOil src/MaizeOilApp.cpp
        src/Playground.h
)

target_link_libraries(MaizeOil PRIVATE Maize)