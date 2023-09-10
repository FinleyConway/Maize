#pragma once

#include "portable-file-dialogs.h"
#include "Tileset.h"

namespace Maize {

    class TilesetWindow
    {
    public:
        TilesetWindow();

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
        std::vector<Tileset> m_Tilesets;
        Tileset* m_SelectedTileset = nullptr;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize