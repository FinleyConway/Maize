#pragma once

#include <Maize.h>

namespace Maize {

    class TilesetWindowTab
    {
    public:
        explicit TilesetWindowTab(std::unordered_map<int32_t, Tileset>& tilesets);

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
        std::unordered_map<int32_t, Tileset>& m_Tilesets;
        Tileset* m_SelectedTileset = nullptr;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize