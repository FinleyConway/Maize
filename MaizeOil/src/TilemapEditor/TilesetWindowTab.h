#pragma once

#include <Maize.h>

namespace Maize {

    class TilesetWindowTab
    {
    public:
		TilesetWindowTab();

        void Window(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);

    private:
        int32_t CreateID() // temp, will create a better id
        {
            static int32_t id = 0;
            return id++;
        }

        Tileset& AddTileset(TilemapEditorWindow::Tilesets& tilesets);
        void RemoveTileset(int32_t tilesetID, TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);

        void SelectTileset(TilemapEditorWindow::Tilesets& tilesets);
        void ShowCurrentTileset(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);
        void TextureSelector(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);
        void SetAutomaticTiles(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);
        void HandleTilesetTiles();

		void UpdateMap(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);
		void PackTileset(TilemapEditorWindow::Tilesets& tilesets, TilemapComponent* tilemapComponent);

    private:
        Tileset* m_SelectedTileset = nullptr;
		float m_TilesetZoomFactor = 4.0f;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize