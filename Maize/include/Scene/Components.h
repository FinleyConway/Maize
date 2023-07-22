#pragma once

#include <EntityWorld.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <map>

#include "Core/AnimationClip.h"
#include "Core/SoundClip.h"
#include "Core/Texture.h"
#include "Core/Colour.h"
#include "Core/Point.h"
#include "Core/Rect.h"

#include "Scene/InputHandler.h"

namespace Maize {

	struct TransformComponent
	{
		PointF position = { 0, 0 };
		float angle = 0;
		PointF scale = { 1, 1 };
	};

	struct SpriteComponent
	{
		const Sprite* sprite = nullptr;
		Colour colour = Colour(255, 255, 255);
		bool flipX = false;
		bool flipY = false;
	};

	struct AnimationComponent
	{
		float animationSpeed = 1;
		uint32_t currentFrame = 0;
		std::string currentState = "";
		std::unordered_map<std::string, AnimationClip> states;
	};

	struct CameraComponent
	{
		enum class RenderMode { NonPixelPerfect, PixelPerfect, };

		RenderMode renderMode = RenderMode::NonPixelPerfect;
		Rect viewport;
		Point bounds;
		float size = 1;
	};

	struct RigidbodyComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };

		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* body = nullptr;
	};

	struct SquareColliderComponent
	{
		PointF offset = { 0, 0 };
		PointF size = PointF(0.5f, 0.5f);

		bool isTrigger = false;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CircleColliderComponent
	{
		PointF offset = { 0, 0 };
		float radius = 0.5f;

		bool isTrigger = false;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CollisionContactComponent
	{
		enum class CollidingEvent { Enter, Exit };

		CollidingEvent collideEvent = CollidingEvent::Enter;
		ECS::Entity entity;
		void* body = nullptr;
	};

	struct AudioSourceComponent
	{
		std::vector<const SoundClip*> audioClips; // queue multiple sounds each frame

		uint8_t volume = 128; // min: 0 max: 128
		bool mute = false;
		bool loop = false;

		bool spatial = false;
		float minDistance = 1;
		float maxDistance = 255; 
	};

	struct AudioListenerComponent
	{
		bool pauseAudio = false;
		uint8_t masterVolume = 128; // min: 0 max: 128
	};

	struct InputComponent
	{
		InputHandler& input;
	};

	struct DummyComponent 
	{ 
		const SoundClip* clip;
	};

}