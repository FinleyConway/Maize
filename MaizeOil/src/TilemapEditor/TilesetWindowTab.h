#pragma once

#include <Maize.h>

#include "Tileset.h"
#include "TilemapEditorTile.h"

namespace Maize {

    class TilesetWindowTab
    {
    public:
		TilesetWindowTab();

        void Window(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);

    private:
        int32_t CreateID() // temp, will create a better id
        {
            static int32_t id = 0;
            return id++;
        }

        Tileset& AddTileset(std::unordered_map<int32_t, Tileset>& tilesets);
        void RemoveTileset(int32_t tilesetID, std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);

        void SelectTileset(std::unordered_map<int32_t, Tileset>& tilesets);
        void ShowCurrentTileset(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);
        void TextureSelector(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);
        void SetAutomaticTiles(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);
        void HandleTilesetTiles();

		void UpdateMap(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);
		void PackTileset(std::unordered_map<int32_t, Tileset>& tilesets, TilemapComponent* tilemapComponent);

    private:
        Tileset* m_SelectedTileset = nullptr;
		float m_TilesetZoomFactor = 4.0f;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize