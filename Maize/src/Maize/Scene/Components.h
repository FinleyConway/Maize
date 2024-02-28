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

	struct Transform
	{
		Vector2 position = Vector2(0.0f, 0.0f);
		float angle = 0.0f;
		Vector2 scale = Vector2(1.0f, 1.0f);
	};

	struct LocalTransform
	{
		Vector2 position = Vector2(0.0f, 0.0f);
		float angle = 0.0f;
		Vector2 scale = Vector2(1.0f, 1.0f);
	};

	struct Relationship
	{
		entt::entity firstChild = entt::null;
		entt::entity prev = entt::null;
		entt::entity next = entt::null;
		entt::entity parent = entt::null;
	};

	struct SpriteRenderer
	{
		Sprite sprite;
		bool flipX = false;
		bool flipY = false;
		int32_t sortingLayer = 0;
		int32_t orderInLayer = 0;
	};

	struct Animator
	{
		float animationSpeed = 1.0f;
		std::string currentState;
		std::unordered_map<std::string, Animation> states;
	};

	struct Rigidbody
	{
		BodyType type = BodyType::Static;
		float gravityScale = 1;
		bool fixedRotation = false;
		bool isContinuous = false;

		b2Body* body = nullptr;
	};

	struct BoxCollider
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

	struct CircleCollider
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

	struct TriggerEnterContact
	{
		entt::entity otherEntity = entt::null;
	};

	struct TriggerExitContact
	{
		entt::entity otherEntity = entt::null;
	};

	struct CollisionEnterContact
	{
		entt::entity otherEntity = entt::null;
	};

	struct CollisionExitContact
	{
		entt::entity otherEntity = entt::null;
	};

} // Maize