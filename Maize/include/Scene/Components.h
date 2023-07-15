#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>

#include "Core/Point.h"
#include "Core/Rect.h"
#include "Core/Texture.h"
#include "Core/AnimationClip.h"

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
		SDL_Colour colour = { 255, 255, 255, 255 };
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
		float zoom = 1;
	};

	struct RigidbodyComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };

		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* body = nullptr;
	};

	struct SquareCollider
	{
		PointF offset = { 0, 0 };
		PointF size = PointF(0.5f, 0.5f);

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CircleCollider
	{
		PointF offset = { 0, 0 };
		float radius = 0.5f;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct AudioSource
	{

	};

}