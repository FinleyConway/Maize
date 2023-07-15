#include <iostream>
#include <algorithm>

#include <EntityWorld.h>

#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/Point.h"
#include "Core/AnimationClip.h"

#include "Scene/Components.h"
#include "Scene/SpriteSheetManager.h"
#include "Scene/Systems/AnimationSystem.h"
#include "Scene/Systems/RenderingSystem.h"
#include "Scene/Systems/PhysicsSystem.h"

using namespace Maize;
using namespace ECS;

auto CreateTestEntity(ECS::EntityWorld& world, PointF position, const Sprite* sprite, const AnimationClip& idle, const AnimationClip& walking, auto type)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& spriteC = world.AddComponent<SpriteComponent>(entity);
	auto& anim = world.AddComponent<AnimationComponent>(entity);
	auto& rigidbody = world.AddComponent<RigidbodyComponent>(entity);
	auto& square = world.AddComponent<SquareCollider>(entity);

	transform.position = position;

	spriteC.sprite = sprite;

	anim.animationSpeed = 1;
	anim.currentState = "Idle";

	anim.states["Idle"] = idle;
	anim.states["Walking"] = walking;

	rigidbody.type = type;
	rigidbody.fixedRotation = false;

	square.size = Point(1, 1);
}

auto CreateCameraEntity(ECS::EntityWorld& world, const Window& window)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& camera = world.AddComponent<CameraComponent>(entity);

	transform.position = PointF(0, 0);

	camera.bounds = window.Size();
	camera.zoom = 10;

	return entity;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return 1; }

	Window window("Test", { 100, 100 }, { 1280, 720 }, 0);
	Renderer renderer(window);
	renderer.SetViewport(Point(0, 0), Point(1280, 720));
	renderer.SetLogicalSize(Point(320, 180));

	SpriteSheetManager spriteManager(renderer);
	spriteManager.AddSpritesFromSheet({ 0, 0 }, { 3, 0 }, { 32, 32 }, { 0, 0 }, 32, "Assets/AnimationTest.png", "PlayerIdle");
	spriteManager.AddSpritesFromSheet({ 0, 1 }, { 3, 1 }, { 32, 32 }, { 0, 0 }, 32, "Assets/AnimationTest.png", "PlayerWalking");

	AnimationClip playerIdle;
	playerIdle.AddFrame(0, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), 100);
	playerIdle.AddFrame(1, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle1"), 100);
	playerIdle.AddFrame(2, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle2"), 100);
	playerIdle.AddFrame(3, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle1"), 100);
	playerIdle.AddFrame(4, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), 100);
	playerIdle.AddFrame(5, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle1"), 100);
	playerIdle.AddFrame(6, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle2"), 100);
	playerIdle.AddFrame(7, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle1"), 100);

	AnimationClip playerWalking;
	playerWalking.AddFrame(0, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking0"), 100);
	playerWalking.AddFrame(1, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking1"), 100);
	playerWalking.AddFrame(2, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking2"), 100);
	playerWalking.AddFrame(3, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking1"), 100);
	playerWalking.AddFrame(4, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking0"), 100);
	playerWalking.AddFrame(5, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking1"), 100);
	playerWalking.AddFrame(6, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking2"), 100);
	playerWalking.AddFrame(7, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerWalking1"), 100);

	EntityWorld world;
	world.RegisterComponent<TransformComponent>();
	world.RegisterComponent<SpriteComponent>();
	world.RegisterComponent<AnimationComponent>();
	world.RegisterComponent<CameraComponent>();
	world.RegisterComponent<RigidbodyComponent>();
	world.RegisterComponent<SquareCollider>();
	world.RegisterComponent<CircleCollider>();

	CreateTestEntity(world, { 0, 0 }, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), playerIdle, playerWalking, RigidbodyComponent::BodyType::Dynamic);
	CreateTestEntity(world, { 0, 5 }, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), playerIdle, playerWalking, RigidbodyComponent::BodyType::Static);

	CreateCameraEntity(world, window);

	// test main loop
	AnimationSystem animationSystem;
	RenderingSystem renderingSystem(renderer);
	PhysicsSystem physicsSystem(PointF(0, 9.8f));

	bool isRunning = true;
	SDL_Event event;
	uint32_t prevTime = SDL_GetTicks();

	physicsSystem.OnStart(world);

	while (isRunning)
	{
		uint32_t currentTime = SDL_GetTicks();
		float deltaTime = static_cast<float>(currentTime - prevTime) / 1000.0f;

		// process events
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isRunning = false;
			}
		}

		physicsSystem.OnUpdate(world, deltaTime);
		animationSystem.OnUpdate(world, deltaTime);
 		renderingSystem.OnRender(world, deltaTime);

		prevTime = currentTime;
	}

	IMG_Quit();
	SDL_Quit();

	return 0;
}