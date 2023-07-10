#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "Core/Renderer.h"
#include "Core/Point.h"
#include "Asset.h"

namespace Maize {

    class AssetManager
    {
    public:
        AssetManager(Renderer& renderer) : m_Renderer(renderer)
        {}

        template <typename T>
        void AddAsset(const std::string& path)
        {
            static_assert(std::is_base_of_v<Asset, T>, "T must be derived from Asset");

            std::hash<std::string> hasher;

            if constexpr (std::is_same_v<T, Texture>)
            {
                if (auto texture = LoadTexture(path); texture.IsValid())
                {
                    m_Assets[hasher(path)] = std::make_unique<Texture>(std::move(texture));
                }
            }
        }

        template <typename T>
        T* GetAsset(const std::string& path) const
        {
            static_assert(std::is_base_of_v<Asset, T>, "T must be derived from Asset");

            std::hash<std::string> hasher;

            if (auto it = m_Assets.find(hasher(path)); it != m_Assets.end())
            {
                Asset* asset = it->second.get();

                if constexpr (std::is_same_v<T, Texture>)
                {
                    return static_cast<T*>(asset);
                }
            }

            return nullptr;
        }

    private:
        std::unordered_map<uint64_t, std::unique_ptr<Asset>> m_Assets;
        Renderer& m_Renderer;


        Texture LoadTexture(const std::string& filePath) const
        {
            // load image
            SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

            if (loadedSurface == nullptr)
            {
                std::cout << SDL_GetError() << std::endl;
                return Texture(); // return an invalid Texture object
            }

            // create texture
            SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
            SDL_FreeSurface(loadedSurface); // free the loaded surface since we no longer need it

            if (newTexture == nullptr)
            {
                std::cout << SDL_GetError() << std::endl;
                return Texture(); // return an invalid Texture object
            }

            return Texture(newTexture, Point( loadedSurface->w, loadedSurface->h ));
        }
    };

}