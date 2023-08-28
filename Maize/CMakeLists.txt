project(src/Maize)

add_library(Maize src/Maize/Core/Application.cpp src/Maize/Core/Application.h
        src/Maize/Core/EntryPoint.h
        src/Maize.h)

target_include_directories(Maize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)