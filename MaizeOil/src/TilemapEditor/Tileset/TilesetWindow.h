#pragma once

#include "portable-file-dialogs.h"
#include "TileAtlas.h"

namespace Maize {

    class TilesetWindow
    {
    public:
        TilesetWindow()
        {
            m_IconAdd = Texture::Create("Resources/Icons/plus.png");
            m_IconRemove = Texture::Create("Resources/Icons/trash-can.png");
        }

        void Window();

    private:
        int32_t CreateID() // temp, will create a better id
        {
            static int32_t id = 0;
            return id++;
        }

        TileAtlas& AddAtlas();
        void RemoveAtlas(int32_t atlasID);

        void SelectAtlas();
        void ShowCurrentAtlas();
        void TextureSelector();
        void SetAutomaticTiles();
        void HandleAtlasTiles();

    private:
        std::vector<TileAtlas> m_Atlases;
        TileAtlas* m_SelectedAtlas = nullptr;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize