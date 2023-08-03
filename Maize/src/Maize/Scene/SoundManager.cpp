#include "Maize/Scene/SoundManager.h"

namespace Maize {

	void SoundManager::AddSound(const std::string& name, const std::string& filePath)
	{
		if (m_Sounds.contains(name))
		{
			std::cout << "Sound already added in sound manager" << std::endl;
		}
		else
		{
			SoundClip clip = LoadSound(filePath);

			if (clip.IsValid())
			{
				m_Sounds[name] = std::move(clip);
			}
		}
	}

	const SoundClip* SoundManager::GetSound(const std::string& name) const
	{
		if (m_Sounds.contains(name))
		{
			return &m_Sounds.at(name);
		}
		else
		{
			std::cout << name << " for sound manager cannot be found!" << std::endl;
			return nullptr;
		}
	}

	void SoundManager::RemoveSound(const std::string& name)
	{
		if (m_Sounds.contains(name))
		{
			m_Sounds.erase(name);
		}
		else
		{
			std::cout << "Trying to remove " << name << " which doesn't exist in sound manager" << std::endl;
		}
	}

	SoundClip SoundManager::LoadSound(const std::string& filePath) const
	{
		SoundClip clip(Mix_LoadWAV(filePath.c_str()));

		if (!clip.IsValid())
		{
			std::cout << Mix_GetError() << " " << filePath << std::endl;
			return SoundClip();
		}

		return clip;
	}

}