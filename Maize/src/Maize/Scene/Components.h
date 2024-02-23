#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "Maize/Renderer/Sprite.h"
#include "Maize/Renderer/Animation.h"

#include "Maize/Math/Vector2.h"
#include "Maize/Physics/Physics.h"
#include "Maize/Physics/PhysicsEngine.h"

/*
 * Backend components
*/

namespace Maize {

	struct TransformComponent
	{
		Vector2 position = Vector2(0.0f, 0.0f);
		float angle = 0.0f;
		Vector2 scale = Vector2(1.0f, 1.0f);
	};

	struct SpriteComponent
	{
		Sprite sprite;
		bool flipX = false;
		bool flipY = false;
		std::string sortingLayer = "Default";
		int32_t orderInLayer = 0;
	};

	struct AnimatorComponent
	{
		float animationSpeed = 1.0f;
		std::string currentState;
		std::unordered_map<std::string, Animation> states;
	};

	struct CameraComponent
	{
		Vector2 zoom = Vector2(1.0f, 1.0f);
		sf::FloatRect viewport = sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f);
	};

	struct RigidbodyComponent
	{
		BodyType type = BodyType::Static;
		float gravityScale = 1;
		bool fixedRotation = false;
		bool isContinuous = false;

		b2Body* body = nullptr;
	};

	struct BoxColliderComponent
	{
		Vector2 offset = Vector2(0.0f, 0.0f);
		Vector2 size = Vector2(1.0f, 1.0f);

		bool isTrigger = false;

		uint16_t categoryBits = 0x0001;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CircleColliderComponent
	{
		Vector2 offset = Vector2(0.0f, 0.0f);
		float radius = 0.5f;

		bool isTrigger = false;

		uint16_t categoryBits = 0x0001;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct TriggerEnterContactComponent
	{
		entt::entity otherEntity = entt::null;
	};

	struct TriggerExitContactComponent
	{
		entt::entity otherEntity = entt::null;
	};

	struct CollisionEnterContactComponent
	{
		entt::entity otherEntity = entt::null;
	};

	struct CollisionExitContactComponent
	{
		entt::entity otherEntity = entt::null;
	};

} // Maize