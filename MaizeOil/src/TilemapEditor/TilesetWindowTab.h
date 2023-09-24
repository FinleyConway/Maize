#pragma once

#include <Maize.h>

namespace Maize {

    class TilesetWindowTab
    {
    public:
		TilesetWindowTab();

		void AddComponent(TilemapComponent* tilemapComponent)
		{
			m_TilemapComponent = tilemapComponent;
		}
        void Window();

    private:
        int32_t CreateID() // temp, will create a better id
        {
            static int32_t id = 0;
            return id++;
        }

        Tileset& AddTileset();
        void RemoveTileset(int32_t tilesetID);

        void SelectTileset();
        void ShowCurrentTileset();
        void TextureSelector();
        void SetAutomaticTiles();
        void HandleTilesetTiles();

    private:
        TilemapComponent* m_TilemapComponent = nullptr;

        Tileset* m_SelectedTileset = nullptr;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize