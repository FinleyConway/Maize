#pragma once

#include "Maize/Math/Vector2.h"
#include "Maize/Audio/AudioClip.h"

namespace Maize
{
	class AudioSource
	{
	 public:
		void SetClip(AudioClip& clip)
		{
			m_Clip = &clip;

			if (!m_Clip->m_IsStreaming)
			{
				auto buffer = static_cast<SoundBufferClip*>(m_Clip->m_Clip.get());
				if (buffer != nullptr)
				{
					m_SoundEffectPlayer.setBuffer(*buffer);
				}
			}
		}

		const AudioClip* GetClip() const
		{
			return m_Clip;
		}

		void SetVolume(float volume);
		float GetVolume() const;
		void SetPitch(float pitch);
		float GetPitch() const;

		void Loop(bool loop)
		{
			if (m_Clip == nullptr) return;

			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					music->setLoop(loop);
				}
			}
			else
			{
				m_SoundEffectPlayer.setLoop(loop);
			}
		}
		bool IsLooping() const
		{
			if (m_Clip == nullptr) return false;

			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					return music->getLoop();
				}
			}
			else
			{
				return m_SoundEffectPlayer.getLoop();
			}
		}

		bool IsPlaying() const;
		void Play();
		void PlayAtPosition(Vector2 position, float depth = 0)
		{
			if (m_Clip == nullptr) return;

			float ppu = 100.0f;
			const int32_t flip = -1; // TODO: temp, probs need a global thing to toggle everywhere that uses this...

			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					music->setPosition(position.x * ppu, position.y * ppu * flip, depth * ppu);
					music->play();
				}
			}
			else
			{
				m_SoundEffectPlayer.setPosition(position.x * ppu, position.y * ppu * flip, depth * ppu);
				m_SoundEffectPlayer.play();
			}
		}

		void Pause();

		void Stop();

		void ToggleMute();
		bool IsMuted() const { return m_IsMuted; }

	 private:
		AudioClip* m_Clip = nullptr;
		sf::Sound m_SoundEffectPlayer;
		float m_PreviousVolume = 0;
		bool m_IsMuted = false;
	};

} // Maize
