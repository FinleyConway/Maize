#pragma once

#include "Maize/Math/Vector2.h"

namespace Maize
{
	class AudioClip;

	class AudioSource
	{
	 public:
		void SetClip(AudioClip& clip);
		const AudioClip* GetClip() const;

		void SetVolume(float volume);
		float GetVolume() const;
		void SetPitch(float pitch);
		float GetPitch() const;

		void Loop(bool loop);
		bool IsLooping() const;

		bool IsPlaying() const;
		void Play();
		void PlayAtPosition(Vector2 position, float depth = 0); // wip

		void Pause();

		void Stop();

		void ToggleMute();
		bool IsMuted() const { return m_IsMuted; }

	 private:
		AudioClip* m_Clip = nullptr;
		sf::Sound m_SoundEffectPlayer;
		float m_PreviousVolume = 0;
		bool m_IsMuted = false;
		bool m_IsPaused = false;
	};

} // Maize
