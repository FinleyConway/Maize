#pragma once

namespace Maize {

    template <typename T>
    class CartesianGrid
    {
    public:
        CartesianGrid() requires std::default_initializable<T> : m_Grid(1) { }
        explicit CartesianGrid(sf::Vector2i size) requires std::default_initializable<T> : m_CurrentSize(size)
        {
            m_Grid.resize(size.x * size.y);
        }

        sf::Vector2i GridSize() const { return m_CurrentSize; }
		void Clear() { m_Grid.clear(); }

        void PushTile(sf::Vector2i position, const T& tile, bool resize = true)
        {
            sf::Vector2i adjustedPosition = AdjustPosition(position);

            if (IsWithinBounds(adjustedPosition))
            {
                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                m_Grid[index] = tile;
            }
            else
            {
                if (!resize) return;

                Resize(adjustedPosition);

                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                m_Grid[index] = tile;
            }
        }

        template <typename... Args>
        void EmplaceTile(sf::Vector2i position, bool resize = true, Args&&... args)
        {
            sf::Vector2i adjustedPosition = AdjustPosition(position);

            if (IsWithinBounds(adjustedPosition))
            {
                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                m_Grid[index] = T(std::forward<Args>(args)...);
            }
            else
            {
                if (!resize) return;

                Resize(adjustedPosition);

                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                m_Grid[index] = T(std::forward<Args>(args)...);
            }
        }

        const T& GetTile(sf::Vector2i position) const
        {
            sf::Vector2i adjustedPosition = AdjustPosition(position);

            if (IsWithinBounds(adjustedPosition))
            {
                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                return m_Grid.at(index);
            }

            return s_DefaultObject;
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

                surrounding.emplace_back(GetTile(sf::Vector2i(x, y)), sf::Vector2i(x, y));
            }

            return surrounding;
        }

        bool ContainsTile(sf::Vector2i position) const
        {
            sf::Vector2i adjustedPosition = AdjustPosition(position);

            if (IsWithinBounds(adjustedPosition))
            {
                return true;
            }

            return false;
        }

        void RemoveTile(sf::Vector2i position)
        {
            sf::Vector2i adjustedPosition = AdjustPosition(position);

            if (IsWithinBounds(adjustedPosition))
            {
                int32_t index = adjustedPosition.y * m_CurrentSize.x + adjustedPosition.x;

                m_Grid[index] = s_DefaultObject;
            }
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

		void Resize(sf::Vector2i adjustedPosition, int32_t resizeAmount = 15)
		{
			int32_t newX = std::max(m_CurrentSize.x, adjustedPosition.x);
			int32_t newY = std::max(m_CurrentSize.y, adjustedPosition.y);

			std::vector<T> newGrid(newX * newY);
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
        std::vector<T> m_Grid;
        sf::Vector2i m_CurrentSize;

        const static T s_DefaultObject;
    };

    template <typename T>
    const T CartesianGrid<T>::s_DefaultObject{};

} // Maize