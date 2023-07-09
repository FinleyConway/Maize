#pragma once

#include <SDL.h>

#include "Math/Vec2.h"
#include "Core/Texture.h"

namespace Maize {

	struct TransformComponent
	{
		Vec2 position = { 0, 0 };
		float angle = 0;
		Vec2 scale = { 1, 1 };
	};

	struct SpriteComponent
	{
		std::string texture = "";
		SDL_Colour colour = { 255, 255, 255, 255 };
		uint32_t pixelPerUnit = 100;
		bool flipX = false;
		bool flipY = false;
	};

	struct Rigidbody
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };

		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* body = nullptr;
	};

	struct SquareCollider
	{
		Vec2 offset = { 0, 0 };
		Vec2 size = Vec2(0.5f, 0.5f);
	};

	struct CircleCollider
	{
		Vec2 offset = { 0, 0 };
		float radius = 0.5f;
	};

	struct AudioSource
	{

	};

	struct CameraComponent
	{
		enum class RenderMode { NonPixelPerfect, PixelPerfect, };

		RenderMode renderMode = RenderMode::NonPixelPerfect;
		SDL_Rect viewport;
		Vec2Int bounds;
		float zoom = 1;
	};

	struct Animator
	{

	};

}