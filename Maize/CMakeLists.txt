project(Maize)

# precompiled header
set(PCH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/mpch.h)
add_library(PrecompiledHeader STATIC ${PCH_HEADER})
set_target_properties(PrecompiledHeader PROPERTIES LINKER_LANGUAGE CXX)

add_library(Maize STATIC
        src/Maize/Core/Application.cpp
        src/Maize/Core/Application.h
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
        src/Maize/Renderer/Renderer.cpp
        src/Maize/Renderer/Renderer.h
        src/Maize/Renderer/Texture.h
        src/Maize/ImGui/ImGuiLayer.cpp
        src/Maize/ImGui/ImGuiLayer.h
        src/Maize/Utils/FileDialog.cpp
        src/Maize/Utils/FileDialog.h
        src/Maize/Utils/SystemNotify.cpp
        src/Maize/Utils/SystemNotify.h
        src/Maize/Scene/Components.h
        src/Maize/Utils/Random.h
        src/Maize/Utils/Random.cpp
        src/Maize/Renderer/TexturePacker.cpp
        src/Maize/Renderer/TexturePacker.h
        src/Maize/Renderer/Sprite.cpp
        src/Maize/Renderer/Sprite.h
        src/Maize/Scene/Systems/RenderingSystem.cpp
        src/Maize/Scene/Systems/RenderingSystem.h
        src/Maize/Renderer/Animation.cpp
        src/Maize/Renderer/Animation.h
        src/Maize/Scene/Systems/AnimationSystem.cpp
        src/Maize/Scene/Systems/AnimationSystem.h
        src/Maize/Scene/Systems/CameraSystem.cpp
        src/Maize/Scene/Systems/CameraSystem.h
        src/Maize/Scene/Systems/CollisionSystem.cpp
        src/Maize/Scene/Systems/CollisionSystem.h
        src/Maize/Math/Math.h
        src/Maize/Math/Vector2.h
        src/Maize/Math/Vector2Int.h
        src/Maize/Math/Vector2.cpp
        src/Maize/Math/Vector2Int.cpp
        src/Maize/Physics/PhysicsEngine.cpp
        src/Maize/Physics/PhysicsEngine.h
        src/Maize/Physics/Physics.cpp
        src/Maize/Physics/Physics.h
)

target_include_directories(Maize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link the precompiled header target to your main project target
target_link_libraries(Maize PUBLIC PrecompiledHeader)
target_precompile_headers(Maize PUBLIC ${PCH_HEADER})
