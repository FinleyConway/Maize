#pragma once

#include "Maize/Renderer/Sprite.h"

namespace Maize {

	class Tile
	{
	public:
		Tile() = default;
		Tile(int32_t tilesetID, int32_t index, Sprite&& sprite) : m_TilesetID(tilesetID), m_Index(index), m_Sprite(std::move(sprite))
		{}

		int32_t GetIndex() const { return m_Index; }
		int32_t GetTilesetID() const { return m_TilesetID; }

		const Sprite& GetSprite() const { return m_Sprite;}
		Sprite& GetSprite() { return m_Sprite; }

	private:
		int32_t m_TilesetID = -1;
		int32_t m_Index = -1;
		Sprite m_Sprite;
	};

} // Maize