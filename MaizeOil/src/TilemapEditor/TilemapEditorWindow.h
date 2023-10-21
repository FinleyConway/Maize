#pragma once

#include <Maize.h>

#include "Tileset.h"
#include "TilemapEditorTile.h"
#include "TilesetWindowTab.h"
#include "TilemapWindowTab.h"

namespace Maize {

    class TilemapEditorWindow
    {
    public:
		void AddComponent(TilemapComponent* tilemapComponent);
        void OnEvent(Event& e);
        void Window();

    private:
		TilemapComponent* m_TilemapComponent = nullptr;
		std::vector<TilemapEditorLayer> m_EditorGrids;
		std::unordered_map<int32_t, Tileset> m_Tilesets;

        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;
    };

} // Maize