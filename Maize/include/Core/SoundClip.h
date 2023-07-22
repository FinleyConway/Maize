#pragma once

#include <SDL_mixer.h>
#include <memory>

namespace Maize {

	struct SoundClip
	{
	public:
		SoundClip() = default;
		explicit SoundClip(Mix_Chunk* chunk) : m_Sound(chunk, &Mix_FreeChunk) { }

		bool IsValid() const
		{
			return m_Sound != nullptr;
		}

		operator Mix_Chunk* () const { return m_Sound.get(); }

	private:
		std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> m_Sound = { nullptr, &Mix_FreeChunk };
	};

}