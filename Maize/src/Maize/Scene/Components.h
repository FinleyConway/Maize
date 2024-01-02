#pragma once

#include <EntityComponentSystem/EntityWorld.h>
#include <box2d/box2d.h>

#include "Maize/Renderer/Sprite.h"
#include "Maize/Utils/Grids/VertexGrid.h"
#include "Maize/Renderer/Animation.h"
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

	struct LocalTransformComponent
	{
		Vector2 position = Vector2(0.0f, 0.0f);
		float angle = 0.0f;
		Vector2 scale = Vector2(1.0f, 1.0f);
	};

	struct ParentComponent
	{
		std::vector<ECS::Entity> children;
	};

	struct ChildComponent
	{
		ECS::Entity parent = -1;
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

	struct CapsuleColliderComponent
	{
		CapsuleDirection direction = CapsuleDirection::Vertical;

		Vector2 offset = Vector2(0.0f, 0.0f);
		Vector2 size = Vector2(0.5f, 1.0f);

		bool isTrigger = false;

		uint16_t categoryBits = 0x0001;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CollisionEventComponent
	{
		ECS::Entity otherEntity = -1;
	};

	struct TilemapComponent
	{
        std::vector<VertexGrid> tilemapLayers;
		std::shared_ptr<Texture> tilemapTexture;
        int32_t tileSizeX = 8;
        int32_t tileSizeY = 8;

        std::string sortingLayer = "Default";
        int32_t orderInLayer = 0;
	};

} // Maize