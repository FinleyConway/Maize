#include <iostream>
#include <algorithm>

#include <EntityWorld.h>

#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/Point.h"

#include "Scene/Components.h"
#include "Scene/Systems/RenderingSystem.h"
#include "Scene/AssetManager.h"

using namespace Maize;
using namespace ECS;

auto CreateTestEntity(ECS::EntityWorld& world, PointF position)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& sprite = world.AddComponent<SpriteComponent>(entity);

	transform.position = position;
	transform.angle = 25;

	sprite.texture = "Assets/Corn.png";
	sprite.pixelPerUnit = 32;
	sprite.flipX = false;
	sprite.flipY = false;

	return entity;
}

auto CreateCameraEntity(ECS::EntityWorld& world, const Window& window)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& camera = world.AddComponent<CameraComponent>(entity);

	transform.position = PointF(0, 0);

	camera.bounds = window.Size();
	camera.zoom = 1;

	return entity;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return 1; }

	Window window("Test", { 100, 100 }, { 1280, 720 }, 0);

	Renderer renderer(window);
	renderer.SetViewport(Point(0, 0), Point(1280, 720));
	renderer.SetLogicalSize({ 320, 180 });

	AssetManager assetManager(renderer);
	assetManager.AddAsset<Texture>("Assets/Corn.png");


	EntityWorld world;
	world.RegisterComponent<TransformComponent>();
	world.RegisterComponent<SpriteComponent>();
	world.RegisterComponent<CameraComponent>();

	auto entity = CreateTestEntity(world, { -40, -40 });
	auto& transform = world.GetComponent<TransformComponent>(entity);

	CreateTestEntity(world, { 100, 100 });

	CreateCameraEntity(world, window);

	RenderingSystem renderingSystem(renderer, assetManager);

	bool isRunning = true;
	SDL_Event event;
	uint32_t prevTime = SDL_GetTicks();

	while (isRunning)
	{
		uint32_t currentTime = SDL_GetTicks();
		float deltaTime = static_cast<float>(currentTime - prevTime) / 1000.0f;

		// start timer
		uint32_t frameStartTime = SDL_GetTicks();

		// process events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}

 		renderingSystem.OnRender(world, deltaTime);

		prevTime = currentTime;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}