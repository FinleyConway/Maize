#pragma once

#include <SDL.h>

#include "Core/Point.h"
#include "Core/Texture.h"

namespace Maize {

	struct TransformComponent
	{
		PointF position = { 0, 0 };
		float angle = 0;
		PointF scale = { 1, 1 };
	};

	struct SpriteComponent
	{
		std::string texture = "";
		std::string name = "";
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
		PointF offset = { 0, 0 };
		PointF size = PointF(0.5f, 0.5f);
	};

	struct CircleCollider
	{
		PointF offset = { 0, 0 };
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
		Point bounds;
		float zoom = 1;
	};

	struct Animator
	{

	};

}