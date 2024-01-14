#include "mpch.h"
#include "Maize/Audio/AudioClip.h"

namespace Maize {

	AudioClip::AudioClip(const std::string& filePath, size_t streamingThreshold)
	{
		auto path = std::filesystem::path(filePath);

		if (!std::filesystem::exists(path))
		{
			std::cerr << "This file path " << filePath << " doesn't exist!" << std::endl;
			return;
		}

		size_t fileSize = std::filesystem::file_size(path);

		if (fileSize > streamingThreshold)
		{
			m_Clip = std::make_shared<MusicClip>();

			static_cast<MusicClip*>(m_Clip.get())->openFromFile(filePath);

			m_IsStreaming = true;
		}
		else
		{
			m_Clip = std::make_shared<SoundBufferClip>();

			static_cast<SoundBufferClip*>(m_Clip.get())->loadFromFile(filePath);
		}
	}

} // Maize