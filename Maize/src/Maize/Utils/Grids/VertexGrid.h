#pragma once

namespace Maize {

	class VertexGrid
	{
	public:
		VertexGrid() = default;
		explicit VertexGrid(sf::Vector2i size) : m_Grid(sf::Quads, size.x * size.y * 4), m_CurrentSize(size)
		{
		}

		sf::VertexArray& GetGrid() { return m_Grid;}
		const sf::VertexArray& GetGrid() const { return m_Grid;}
		sf::Vector2i GridSize() const { return m_CurrentSize; }
		void Clear() { m_Grid.clear(); }

		void PushTile(sf::Vector2i position, const std::array<sf::Vertex, 4> quad, bool resize = true)
		{
			sf::Vector2i adjustedPosition = AdjustPosition(position);

			if (IsWithinBounds(adjustedPosition))
			{
				uint32_t index = (adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x) * 4;

				for (uint32_t i = 0; i < quad.size(); i++)
					m_Grid[index + i] = quad[i];
			}
			else
			{
				return;
				if (!resize) return;

				Resize(adjustedPosition);

				uint32_t index = (adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x) * 4;

				for (uint32_t i = 0; i < quad.size(); i++)
					m_Grid[index + i] = quad[i];
			}
		}

		void RemoveTile(sf::Vector2i position)
		{
			sf::Vector2i adjustedPosition = AdjustPosition(position);

			if (IsWithinBounds(adjustedPosition))
			{
				uint32_t index = (adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x) * c_VertexCount;

				for (uint32_t i = 0; i < c_VertexCount; i++)
					m_Grid[index + i] = s_DefaultObject;
			}
		}

		void Resize(sf::Vector2i adjustedPosition, int32_t resizeAmount = 15)
		{
			int32_t newX = std::max(m_CurrentSize.x, adjustedPosition.x);
			int32_t newY = std::max(m_CurrentSize.y, adjustedPosition.y);

			sf::VertexArray newGrid(sf::Quads, newX * newY * c_VertexCount);
		}

	private:
		sf::Vector2i AdjustPosition(sf::Vector2i position) const // 20x20 grid (0, 0) -> (10, 10)
		{
			return sf::Vector2i(position.x + m_CurrentSize.x / 2, position.y + m_CurrentSize.y / 2);
		}

		bool IsWithinBounds(sf::Vector2i adjustedPosition) const
		{
			return adjustedPosition.x >= 0 && adjustedPosition.x < m_CurrentSize.x &&
				   adjustedPosition.y >= 0 && adjustedPosition.y < m_CurrentSize.y;
		}

	private:
		sf::VertexArray m_Grid;
		sf::Vector2i m_CurrentSize;
		const uint8_t c_VertexCount = 4;
		static sf::Vertex s_DefaultObject;
	};

} // Maize