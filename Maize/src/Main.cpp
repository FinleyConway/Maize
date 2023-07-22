#include <iostream>
#include <algorithm>

#include <EntityWorld.h>
#include <SDL_mixer.h>

#include "Core/Window.h"
#include "Core/Renderer.h"
#include "Core/Point.h"
#include "Core/AnimationClip.h"

#include "Scene/Components.h"
#include "Scene/SpriteSheetManager.h"
#include "Scene/SoundManager.h"
#include "Scene/InputHandler.h"
#include "Scene/Systems/AnimationSystem.h"
#include "Scene/Systems/RenderingSystem.h"
#include "Scene/Systems/PhysicsSystem.h"
#include "Scene/Systems/AudioSystem.h"
#include "Scene/Systems/TestCollisionEvent.h"

using namespace Maize;
using namespace ECS;

auto CreateTestEntity(ECS::EntityWorld& world, PointF position, const Sprite* sprite, const AnimationClip& idle, const AnimationClip& walking, auto type)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& spriteC = world.AddComponent<SpriteComponent>(entity);
	auto& anim = world.AddComponent<AnimationComponent>(entity);
	auto& rigidbody = world.AddComponent<RigidbodyComponent>(entity);
	auto& square = world.AddComponent<SquareColliderComponent>(entity);

	transform.position = position;

	spriteC.sprite = sprite;

	anim.animationSpeed = 1;
	anim.currentState = "Idle";

	anim.states["Idle"] = idle;
	anim.states["Walking"] = walking;

	rigidbody.type = type;
	rigidbody.fixedRotation = false;

	square.size = Point(1, 1);

	return entity;
}

auto CreateCameraEntity(ECS::EntityWorld& world, const Window& window)
{
	auto entity = world.CreateEntity();
	auto& transform = world.AddComponent<TransformComponent>(entity);
	auto& camera = world.AddComponent<CameraComponent>(entity);

	transform.position = PointF(0, 0);

	camera.viewport = Rect(0, 0, 1280, 720);
	camera.bounds = window.Size();

	return entity;
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return 1; 
	if (~IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) return 1;
	if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) return 1;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 1;

	Window window("Test", { 100, 100 }, { 1280, 720 }, 0);
	Renderer renderer(window);
	renderer.SetViewport(Point(0, 0), Point(1280, 720));

	SoundManager soundManager;
	soundManager.AddSound("Fart", "Assets/perfect-fart.mp3");

	SpriteSheetManager spriteManager(renderer);
	spriteManager.AddSpritesFromSheet({ 0, 0 }, { 3, 0 }, { 32, 32 }, { 16, 16 }, 32, "Assets/AnimationTest.png", "PlayerIdle");
	spriteManager.AddSpritesFromSheet({ 0, 1 }, { 3, 1 }, { 32, 32 }, { 16, 16 }, 32, "Assets/AnimationTest.png", "PlayerWalking");

	InputHandler inputHandler;

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
	world.RegisterComponent<SquareColliderComponent>();
	world.RegisterComponent<CircleColliderComponent>();
	world.RegisterComponent<CollisionContactComponent>();
	world.RegisterComponent<AudioListenerComponent>();
	world.RegisterComponent<AudioSourceComponent>();
	world.RegisterComponent<DummyComponent>();

	auto entity = CreateTestEntity(world, { 0, 0 }, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), playerIdle, playerWalking, RigidbodyComponent::BodyType::Dynamic);
	auto& dummy = world.AddComponent<DummyComponent>(entity);
	dummy.clip = soundManager.GetSound("Fart");

	auto entity2 = CreateTestEntity(world, { 0, 1.5f }, spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0"), playerIdle, playerWalking, RigidbodyComponent::BodyType::Static);
	auto& sound = world.AddComponent<AudioSourceComponent>(entity2);
	sound.spatial = true;
	sound.minDistance = 1;
	sound.maxDistance = 9;

	auto camera = CreateCameraEntity(world, window);
	world.AddComponent<AudioListenerComponent>(camera);
	world.AddComponent<SpriteComponent>(camera).sprite = spriteManager.GetSprite("Assets/AnimationTest.png", "PlayerIdle0");
	auto& ad = world.GetComponent<CameraComponent>(camera);
	ad.size = 10;

	// test main loop
	AnimationSystem animationSystem;
	RenderingSystem renderingSystem(renderer);
	PhysicsSystem physicsSystem(world, PointF(0, 9.81f));
	AudioSystem audioSystem;
	TestCollisionEvent testCollisionEvent;

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

			inputHandler.PollEvent(event);
		}

		physicsSystem.OnUpdate(world, deltaTime);

		testCollisionEvent.OnUpdate(world, deltaTime);

		PointF direction = PointF(0, 0);
		if (inputHandler.GetKey(SDL_SCANCODE_W))
		{
			direction.y -= 2;
		}
		if (inputHandler.GetKey(SDL_SCANCODE_S))
		{
			direction.y += 2;
		}
		if (inputHandler.GetKey(SDL_SCANCODE_A))
		{
			direction.x -= 2;
		}
		if (inputHandler.GetKey(SDL_SCANCODE_D))
		{
			direction.x += 2;
		}
		world.GetComponent<TransformComponent>(camera).position += direction * deltaTime * 5;

		animationSystem.OnUpdate(world, deltaTime);
		audioSystem.OnUpdate(world, deltaTime);

 		renderingSystem.OnRender(world, deltaTime);

		physicsSystem.OnEndFrame(world);
		prevTime = currentTime;
	}

	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}