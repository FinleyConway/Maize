project(MaizeOil)

add_executable(MaizeOil src/MaizeOilApp.cpp)

target_link_libraries(MaizeOil PRIVATE Maize)