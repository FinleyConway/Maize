#include "mpch.h"
#include "Maize/Utils/Grids/VertexGrid.h"

namespace Maize {

	VertexGrid::VertexGrid(sf::Vector2i size) : m_Grid(sf::Quads, size.x * size.y * 4), m_CurrentSize(size)
	{
	}

	void VertexGrid::InsertTile(sf::Vector2i position, const std::array<sf::Vertex, 4>& quad, bool resize)
	{
		int32_t index = GetIndex(position, resize);

		if (index != -1)
		{
			for (uint8_t i = 0; i < quad.size(); i++)
				m_Grid[index + i] = quad[i];
		}
	}

	std::array<sf::Vertex*, 4> VertexGrid::GetTile(sf::Vector2i position)
	{
		std::array<sf::Vertex*, 4> quad{};

        if (!IsOutOfBounds(position))
        {
            sf::Vector2i adjusted = AdjustPosition(position);
            int32_t index = (adjusted.y * m_CurrentSize.x + adjusted.x) * c_VertexCount;

            for (uint8_t i = 0; i < c_VertexCount; i++)
                quad[i] = &m_Grid[index + i];

            return quad;
        }

		return quad;
	}

	bool VertexGrid::ContainsTile(sf::Vector2i position) const
	{
        if (!IsOutOfBounds(position))
        {
            return true;
        }

        return false;
	}

	void VertexGrid::RemoveTile(sf::Vector2i position)
	{
        if (!IsOutOfBounds(position))
        {
            sf::Vector2i adjusted = AdjustPosition(position);
            int32_t index = (adjusted.y * m_CurrentSize.x + adjusted.x) * c_VertexCount;

            for (uint8_t i = 0; i < c_VertexCount; i++)
                m_Grid[index + i] = sf::Vertex();
        }
	}

	void VertexGrid::Resize(int32_t newWidth, int32_t newHeight)
	{
		sf::VertexArray newGrid(sf::Quads, newWidth * newHeight * c_VertexCount);

		for (int32_t y = 0; y < newHeight; y++)
		{
			for (int32_t x = 0; x < newWidth; x++)
			{
				int32_t newIndex = (y * newWidth + x) * c_VertexCount;
				int32_t oldX = x - (newWidth - m_CurrentSize.x) / 2;
				int32_t oldY = y - (newHeight - m_CurrentSize.y) / 2;
				int32_t oldIndex = (oldY * m_CurrentSize.x + oldX) * c_VertexCount;

				// add new tiles when its within new old grid
				if (oldX >= 0 && oldX < m_CurrentSize.x && oldY >= 0 && oldY < m_CurrentSize.y)
				{
					for (uint8_t i = 0; i < c_VertexCount; i++)
					{
						newGrid[newIndex + i] = m_Grid[oldIndex + i];
					}
				}
				else
				{
					for (uint8_t i = 0; i < c_VertexCount; i++)
					{
						newGrid[newIndex + i] = sf::Vertex();
					}
				}
			}
		}

		m_Grid = std::move(newGrid);
		m_CurrentSize = sf::Vector2i(newWidth, newHeight);
	}


	int32_t VertexGrid::GetIndex(sf::Vector2i position, bool resize)
	{
		if (IsOutOfBounds(position))
		{
			if (!resize) return -1;

			int32_t newWidth = std::max(m_CurrentSize.x, std::abs(position.x) * 15 + 1);
			int32_t newHeight = std::max(m_CurrentSize.y, std::abs(position.y) * 15 + 1);

			Resize(newWidth, newHeight);
		}

        sf::Vector2i adjusted = AdjustPosition(position);

		return (adjusted.y * m_CurrentSize.x + adjusted.x) * c_VertexCount;
	}

    sf::Vector2i VertexGrid::AdjustPosition(sf::Vector2i position) const
    {
        return sf::Vector2i(position.x + m_CurrentSize.x / 2, position.y + m_CurrentSize.y / 2);
    }

	bool VertexGrid::IsOutOfBounds(sf::Vector2i position) const
	{
		return position.x < -m_CurrentSize.x / 2 || position.x >= m_CurrentSize.x / 2 ||
			   position.y < -m_CurrentSize.y / 2 || position.y >= m_CurrentSize.y / 2;
	}

} // Maize