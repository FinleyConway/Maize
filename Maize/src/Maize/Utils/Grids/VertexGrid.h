#pragma once

namespace Maize {

	class VertexGrid
	{
	public:
		VertexGrid() = default;
		explicit VertexGrid(sf::Vector2i size);

		const sf::VertexArray& GetGrid() const { return m_Grid;}
		sf::Vector2i GridSize() const { return m_CurrentSize; }
		void Clear() { m_Grid.clear(); }

		void InsertTile(sf::Vector2i position, const std::array<sf::Vertex, 4>& quad, bool resize = true);
		std::array<sf::Vertex, 4> GetTile(sf::Vector2i position);
		bool ContainsTile(sf::Vector2i position);
		void RemoveTile(sf::Vector2i position);
		void Resize(int32_t newWidth, int32_t newHeight);

	private:
		int32_t GetIndex(sf::Vector2i position, bool resize = true);
		bool IsOutOfBounds(sf::Vector2i position) const;

	private:
		sf::VertexArray m_Grid;
		sf::Vector2i m_CurrentSize;
		const uint8_t c_VertexCount = 4;
		static sf::Vertex s_DefaultObject;
	};

} // Maize