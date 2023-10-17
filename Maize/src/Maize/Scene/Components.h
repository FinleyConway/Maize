#pragma once

#include "Maize/Tilemap/TilemapLayer.h"
#include "Maize/Tilemap/Tileset.h"
#include "Maize/Renderer/Sprite.h"

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

	struct SpriteComponent
	{
		Sprite sprite;
		std::string sortingLayer = "Default";
		int32_t orderInLayer = 0;
	};

	struct TilemapComponent
	{
        std::unordered_map<int32_t, Tileset> tilesets;

		std::vector<TilemapLayer> layers;
		int32_t tileSizeX = 8;
		int32_t tileSizeY = 8;

		std::shared_ptr<sf::Texture> texture;
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