#pragma once

namespace Maize {

    template <typename T>
    class CartesianGrid
    {
    public:
        CartesianGrid() requires std::default_initializable<T> = default;
        explicit CartesianGrid(sf::Vector2i size) requires std::default_initializable<T> : m_Grid(size.x * size.y) { }

        sf::Vector2i GridSize() const { return m_CurrentSize; }
		void Clear() { m_Grid.clear(); }

        void PushTile(sf::Vector2i position, const T& tile, bool resize = true)
        {
			int32_t index = GetIndex(position, resize);

			if (index != -1)
			{
				m_Grid[index] = tile;
			}
        }

        template <typename... Args>
        void InsertTile(sf::Vector2i position, bool resize = true, Args&&... args)
        {
			int32_t index = GetIndex(position, resize);

			if (index != -1)
			{
				m_Grid[index] = T(std::forward<Args>(args)...);
			}
        }

        const T* GetTile(sf::Vector2i position) const
        {
            if (!IsOutOfBounds(position))
            {
                sf::Vector2i adjusted = AdjustPosition(position);

                return &m_Grid.at(adjusted.y * m_CurrentSize.x + adjusted.x);
            }

            return nullptr;
        }

        std::vector<std::pair<const T&, sf::Vector2i>> GetSurroundingTiles(sf::Vector2i initPosition) const
        {
            std::vector<std::pair<const T&, sf::Vector2i>> surrounding;

            static std::array<sf::Vector2i, 8> adjacentOffsets = {
                    sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),
                    sf::Vector2i(-1,  0),                      	   sf::Vector2i(1,  0),
                    sf::Vector2i(-1,  1), sf::Vector2i(0,  1), sf::Vector2i(1,  1)
            };

            for (const auto& offset : adjacentOffsets)
            {
                int32_t x = initPosition.x + offset.x;
                int32_t y = initPosition.y + offset.y;

                const T* tile = GetTile(sf::Vector2i(x, y));

                if (tile != nullptr)
                    surrounding.emplace_back(*tile, sf::Vector2i(x, y));
            }

            return surrounding;
        }

        bool ContainsTile(sf::Vector2i position) const
        {
            if (!IsOutOfBounds(position))
            {
                return true;
            }

            return false;
        }

        void RemoveTile(sf::Vector2i position)
        {
            if (!IsOutOfBounds(position))
            {
                sf::Vector2i adjusted = AdjustPosition(position);

                m_Grid[adjusted.y * m_CurrentSize.x + adjusted.x] = T();
            }
        }

		void Resize(int32_t newWidth, int32_t newHeight)
		{
			std::vector<T> newGrid(newWidth * newHeight, T());

			for (int32_t y = 0; y < newHeight; y++)
			{
				for (int32_t x = 0; x < newWidth; x++)
				{
					int32_t oldX = x - (newWidth - m_CurrentSize.x) / 2;
					int32_t oldY = y - (newHeight - m_CurrentSize.y) / 2;

					if (oldX >= 0 && oldX < m_CurrentSize.x && oldY >= 0 && oldY < m_CurrentSize.y)
					{
						newGrid[y * newWidth + x] = m_Grid[oldY * m_CurrentSize.x + oldX];
					}
				}
			}

			m_Grid = std::move(newGrid);
			m_CurrentSize = sf::Vector2i(newWidth, newHeight);
		}

		static sf::Vector2i ConvertScreenToGrid(sf::Vector2f position, sf::Vector2i cellSize)
        {
            return sf::Vector2i(
                    static_cast<int32_t>(std::floor(position.x / static_cast<float>(cellSize.x))),
                    static_cast<int32_t>(std::floor(position.y / static_cast<float>(cellSize.y)))
            );
        }

        static sf::Vector2f ConvertGridToScreen(sf::Vector2i position, sf::Vector2i cellSize)
        {
            return sf::Vector2f(
                    static_cast<float>(position.x) * static_cast<float>(cellSize.x),
                    static_cast<float>(position.y) * static_cast<float>(cellSize.y)
            );
        }

	private:
		int32_t GetIndex(sf::Vector2i position, bool resize)
		{
			if (IsOutOfBounds(position))
			{
                if (!resize) return -1;

				int32_t newWidth = std::max(m_CurrentSize.x, std::abs(position.x) * 2 + 1);
				int32_t newHeight = std::max(m_CurrentSize.y, std::abs(position.y) * 2 + 1);

				Resize(newWidth, newHeight);
			}

            sf::Vector2i adjusted = AdjustPosition(position);

			return adjusted.y * m_CurrentSize.x + adjusted.x;
		}

        sf::Vector2i AdjustPosition(sf::Vector2i position) const
        {
            return sf::Vector2i(position.x + m_CurrentSize.x / 2, position.y + m_CurrentSize.y / 2);
        }

		bool IsOutOfBounds(sf::Vector2i position) const
		{
			return position.x < -m_CurrentSize.x / 2 || position.x >= m_CurrentSize.x / 2 ||
				   position.y < -m_CurrentSize.y / 2 || position.y >= m_CurrentSize.y / 2;
		}

    private:
        std::vector<T> m_Grid;
        sf::Vector2i m_CurrentSize;
    };

} // Maize