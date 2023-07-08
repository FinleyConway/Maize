#include <iostream>
#include <algorithm>

#include <EntityWorld.h>

#include "Core/Window.h"
#include "Core/Renderer.h"

#include "Scene/Components.h"
#include "Scene/Systems/RenderingSystem.h"
#include "Scene/AssetManager.h"

using namespace Maize;
using namespace ECS;

auto CreateTestEntity(ECS::EntityWorld& world, Vec2 position)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& sprite = world.AddComponent<SpriteComponent>(entity);

	transform.position = position;
	transform.angle = 0;

	sprite.texture = "Assets/Corn.png";
	sprite.pixelPerUnit = 32;

	return entity;
}

auto CreateCameraEntity(ECS::EntityWorld& world)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& camera = world.AddComponent<CameraComponent>(entity);

	transform.position = Vec2(0, 0);

	camera.zoom = 1;

	return entity;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return 1; }

	Window window("Test", { 100, 100 }, { 1280, 720 }, 0);

	Renderer renderer(window);
	renderer.SetViewport(Vec2Int(0, 0), Vec2Int(1280, 720));
	renderer.SetLogicalSize({ 320, 180 });

	AssetManager assetManager(renderer);
	assetManager.AddAsset<Texture>("Assets/Corn..png");


	EntityWorld world;
	world.RegisterComponent<TransformComponent>();
	world.RegisterComponent<SpriteComponent>();
	world.RegisterComponent<CameraComponent>();

	for (float i = 0; i < 32*4; i++)
	{
		for (float j = 0; j < 32*4; j++)
		{
			CreateTestEntity(world, { i, j });
		}
	}
	CreateCameraEntity(world);

	RenderingSystem renderingSystem(renderer, assetManager);

	bool isRunning = true;
	SDL_Event event;
	uint32_t prevTime = SDL_GetTicks();

	while (isRunning)
	{
		uint32_t currentTime = SDL_GetTicks();
		float deltaTime = static_cast<float>(currentTime - prevTime) / 1000.0f;

		// Start timer
		uint32_t frameStartTime = SDL_GetTicks();

		// Process events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}

		renderingSystem.OnRender(world, deltaTime);

		// Calculate frame time
		uint32_t frameEndTime = SDL_GetTicks();
		float frameTime = static_cast<float>(frameEndTime - frameStartTime) / 1000.0f;
		std::cout << "Frame Time: " << frameTime << " seconds" << std::endl;

		prevTime = currentTime;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}

/*
TODO:
* start to actually creating a scene rather then slapping everything in main
*/