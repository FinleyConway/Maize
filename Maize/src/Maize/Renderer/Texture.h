#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <memory>

namespace Maize {

    struct Texture
    {
    public:
        Texture() = default;

        static Texture Create(const std::string& filePath)
        {
            Texture texture;

            if (!texture.m_Texture.loadFromFile(filePath))
            {
                std::cerr << "Failed to load texture from file: " << filePath << std::endl;
                texture.m_IsValid = false;
            }
            texture.m_IsValid = true;

            return texture;
        }

        bool IsValid() const { return m_IsValid; }
        uint32_t GetWidth() const { return m_Texture.getSize().x; }
        uint32_t GetHeight() const { return m_Texture.getSize().y; }

        operator sf::Texture& () { return m_Texture; }
        operator const sf::Texture& () const { return m_Texture; }

    private:
        sf::Texture m_Texture;
        bool m_IsValid = false;
    };

}