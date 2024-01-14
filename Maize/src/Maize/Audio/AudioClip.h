#pragma once

#include <filesystem>

namespace Maize
{
	class BaseClip
	{
	 public:
		virtual ~BaseClip() = default;

		virtual float GetLength() const = 0;
	};

 	class MusicClip : public BaseClip, public sf::Music
	{
	 public:
		float GetLength() const override
		{
			return getDuration().asSeconds();
		}
	};

	class SoundBufferClip : public BaseClip, public sf::SoundBuffer
	{
	 public:
		float GetLength() const override
		{
			return getDuration().asSeconds();
		}
	};

	class AudioClip
	{
	 public:
		explicit AudioClip(const std::string& filePath, size_t streamingThreshold = 1048576 /* 1mb */)
		{
			auto path = std::filesystem::path(filePath);

			if (!std::filesystem::exists(path))
			{
				std::cout << "This file path " << filePath << " doesn't exist!" << std::endl;
				return;
			}

			size_t fileSize = std::filesystem::file_size(path);

			if (fileSize > streamingThreshold)
			{
				m_Clip = std::make_shared<MusicClip>();

				if (!static_cast<MusicClip*>(m_Clip.get())->openFromFile(filePath))
				{
					// error
				}

				m_IsStreaming = true;
			}
			else
			{
				m_Clip = std::make_shared<SoundBufferClip>();

				if (!static_cast<SoundBufferClip*>(m_Clip.get())->loadFromFile(filePath))
				{
					// error
				}
			}
		}

		float GetLength() const
		{
			return m_Clip->GetLength();
		}

	 private:
		friend class AudioSource;

		std::shared_ptr<BaseClip> m_Clip;
		bool m_IsStreaming = false;
	};

} // Maize
