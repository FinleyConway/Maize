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
		explicit AudioClip(const std::string& filePath, size_t streamingThreshold = 1048576 /* 1mb */);

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
