project(Maize)

include(FetchContent)

# import external dependencies
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git  GIT_TAG 2.6.1)
FetchContent_Declare(entt GIT_REPOSITORY https://github.com/skypjack/entt.git GIT_TAG v3.12.2)
FetchContent_Declare(box2d GIT_REPOSITORY https://github.com/erincatto/box2d.git GIT_TAG v2.4.1)

# import personal external dependencies
FetchContent_Declare(mix GIT_REPOSITORY https://github.com/FinleyConway/MaizeMix.git GIT_TAG master)

FetchContent_MakeAvailable(sfml)
FetchContent_MakeAvailable(entt)
FetchContent_MakeAvailable(box2d)

FetchContent_MakeAvailable(mix)

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
        src/Maize/Scene/Components.h
        src/Maize/Utils/Random.h
        src/Maize/Utils/Random.cpp
        src/Maize/Renderer/Sprite.cpp
        src/Maize/Renderer/Sprite.h
        src/Maize/Scene/Systems/RenderingSystem.cpp
        src/Maize/Scene/Systems/RenderingSystem.h
        src/Maize/Renderer/Animation.cpp
        src/Maize/Renderer/Animation.h
        src/Maize/Scene/Systems/AnimationSystem.cpp
        src/Maize/Scene/Systems/AnimationSystem.h
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
        src/Maize/Physics/PhysicsContactListener.h
        src/Maize/Scene/Scene.cpp
        src/Maize/Scene/Scene.h
        src/Maize/Physics/PhysicsContactListener.cpp
        src/Maize/Scene/System.h
        src/Maize/Scene/Entity.h
        src/Maize/Scene/Systems/HierarchySystem.cpp
        src/Maize/Scene/Systems/HierarchySystem.h
)

target_include_directories(Maize PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(Maize PUBLIC
        PrecompiledHeader
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        EnTT::EnTT
        box2d
        MaizeMix
)

target_precompile_headers(Maize PUBLIC ${PCH_HEADER})
