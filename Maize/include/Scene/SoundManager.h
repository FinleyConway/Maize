#pragma once

#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>
#include <string>

#include "Core/SoundClip.h"

namespace Maize {

	class SoundManager
	{
	public:
		void AddSound(const std::string& name, const std::string& filePath);
		const SoundClip* GetSound(const std::string& name) const;
		void RemoveSound(const std::string& name);

	private:
		std::unordered_map<std::string, SoundClip> m_Sounds;

		SoundClip LoadSound(const std::string& filePath) const;
	};

}