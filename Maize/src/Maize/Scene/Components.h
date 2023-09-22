#pragma once

#include "Maize/Tilemap/TilemapLayer.h"
#include "Maize/Tilemap/Tileset.h"
#include "Maize/Renderer/Sprite.h"

namespace Maize {

	struct TransformComponent
	{
		sf::Vector2f position;
		float angle = 0;
		sf::Vector2f scale;
	};

	struct SpriteComponent
	{
		Sprite* sprite;
		std::string sortingLayer;
		int32_t orderInLayer = 0;
	};

	struct TilemapComponent
	{
		std::vector<TilemapLayer> layers;
		std::vector<Tileset> tilesets;
		int32_t tileSizeX = 8;
		int32_t tileSizeY = 8;
	};

	struct TilemapColliderComponent
	{

	};

	struct BoxColliderComponent
	{

	};

	struct CircleColliderComponent
	{

	};

	struct CapsuleColliderComponent
	{

	};

} // Maize