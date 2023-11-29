#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Renderer/Sprite.h"
#include "Maize/Utils/Grids/VertexGrid.h"
#include "Maize/Renderer/Animation.h"

/*
 * Backend components
*/

namespace Maize {

	struct TransformComponent
	{
		sf::Vector2f position = sf::Vector2f(0, 0);
		float angle = 0;
		sf::Vector2f scale = sf::Vector2f(1, 1);
	};

	struct LocalTransformComponent
	{
		sf::Vector2f position = sf::Vector2f(0, 0);
		float angle = 0;
		sf::Vector2f scale = sf::Vector2f(1, 1);
	};

	struct ParentComponent
	{
		std::vector<ECS::Entity> children;
	};

	struct ChildComponent
	{
		ECS::Entity parent;
	};

	struct SpriteComponent
	{
		Sprite sprite;
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
		sf::Vector2f zoom = sf::Vector2f(1, 1);
		sf::FloatRect viewport = sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f);
	};

	struct CameraShakeComponent
	{
		float intensity = 0;
		float duration = 0;
		sf::Vector2f originalPosition;

		void Shake(float shakeIntensity, float time, sf::Vector2f startingPosition)
		{
			if (duration > 0) return;

			intensity = shakeIntensity;
			duration = time;
			originalPosition = startingPosition;
		}
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

	struct RigidbodyComponent
	{
		enum class BodyType { Static = 0, Kinematic, Dynamic };

		BodyType type = BodyType::Static;
		bool fixedRotation = false;
		void* body = nullptr;
	};

	struct TilemapColliderComponent
	{
	};

	struct BoxColliderComponent
	{
		sf::Vector2f offset;
		sf::Vector2f size = sf::Vector2f(0.5f, 0.5f);

		bool isTrigger = false;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CircleColliderComponent
	{
		sf::Vector2f offset;
		float radius = 0.5f;

		bool isTrigger = false;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

	struct CapsuleColliderComponent
	{
		sf::Vector2f offset;
		sf::Vector2f size = sf::Vector2f(0.5f, 1.0f);

		bool isTrigger = false;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		float restitutionThreshold = 0.5f;
	};

} // Maize