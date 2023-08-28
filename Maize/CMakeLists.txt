project(src/Maize)

add_library(Maize src/Maize/Core/Application.cpp src/Maize/Core/Application.h
        src/Maize/Core/EntryPoint.h
        src/Maize.h
        src/Maize/Core/Window.cpp
        src/Maize/Core/Window.h
        src/Maize/Core/Layer.h
        src/Maize/Core/LayerStack.cpp
        src/Maize/Core/LayerStack.h
        src/Maize/Events/Event.h
        src/Maize/Events/EventDispatcher.h
        src/Maize/Events/WindowEvents.h
        src/Maize/Core/KeyCodes.h
        src/Maize/Events/KeyEvents.h
        src/Maize/Events/MouseEvents.h
        src/Maize/Core/Input.h
        src/Maize/Math/Point.cpp
        src/Maize/Math/Point.h
        src/Maize/Math/PointF.cpp
        src/Maize/Math/PointF.h
        src/Maize/Math/Rect.cpp
        src/Maize/Math/Rect.h
        src/Maize/Math/RectF.cpp
        src/Maize/Math/RectF.h
        src/Maize/Renderer/Renderer.cpp
        src/Maize/Renderer/Renderer.h
        src/Maize/Renderer/Texture.h
        src/Maize/Renderer/Sprite.h
        src/Maize/Renderer/Colour.cpp
        src/Maize/Renderer/Colour.h)

target_include_directories(Maize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
