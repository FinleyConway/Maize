#include "mpch.h"
#include "Maize/Audio/AudioSource.h"
#include "Maize/Audio/AudioClip.h"

namespace Maize {

	void AudioSource::SetClip(AudioClip& clip)
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

	const AudioClip* AudioSource::GetClip() const
	{
		return m_Clip;
	}

	void AudioSource::SetVolume(float volume)
	{
		if (m_Clip == nullptr) return;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			if (music != nullptr)
			{
				music->setVolume(volume);
			}
		}
		else
		{
			m_SoundEffectPlayer.setVolume(volume);
		}
	}

	float AudioSource::GetVolume() const
	{
		if (m_Clip == nullptr) return 0.0f;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			return (music != nullptr) ? music->getVolume() : 0.0f;
		}
		else
		{
			return m_SoundEffectPlayer.getVolume();
		}
	}

	void AudioSource::SetPitch(float pitch)
	{
		if (m_Clip == nullptr) return;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			if (music != nullptr)
			{
				music->setPitch(pitch);
			}
		}
		else
		{
			m_SoundEffectPlayer.setPitch(pitch);
		}
	}

	float AudioSource::GetPitch() const
	{
		if (m_Clip == nullptr) 0.0f;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			if (music != nullptr)
			{
				return music->getPitch();
			}
		}
		else
		{
			return m_SoundEffectPlayer.getPitch();
		}

		return 0.0f; // hush linker warning
	}

	void AudioSource::Loop(bool loop)
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

	bool AudioSource::IsLooping() const
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

		return false; // hush linker warning
	}

	bool AudioSource::IsPlaying() const
	{
		if (m_Clip == nullptr) return false;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			return (music != nullptr) ? (music->getStatus() == sf::SoundSource::Playing) : false;
		}
		else
		{
			return m_SoundEffectPlayer.getStatus() == sf::SoundSource::Playing;
		}

		return false; // hush linker warning
	}

	void AudioSource::Play()
	{
		if (m_Clip == nullptr) return;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			if (music != nullptr)
			{
				music->play();
			}
		}
		else
		{
			m_SoundEffectPlayer.play();
		}
	}

	void AudioSource::PlayAtPosition(Vector2 position, float depth)
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

	void AudioSource::Pause()
	{
		if (m_Clip == nullptr) return;

		if (m_IsPaused)
		{
			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					music->play();
				}
			}
			else
			{
				m_SoundEffectPlayer.play();
			}

			m_IsPaused = false;
		}
		else
		{
			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					music->pause();
				}
			}
			else
			{
				m_SoundEffectPlayer.pause();
			}

			m_IsPaused = true;
		}
	}

	void AudioSource::Stop()
	{
		if (m_Clip == nullptr) return;

		if (m_Clip->m_IsStreaming)
		{
			auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
			if (music != nullptr)
			{
				music->stop();
			}
		}
		else
		{
			m_SoundEffectPlayer.stop();
		}
	}

	void AudioSource::ToggleMute()
	{
		if (m_Clip == nullptr) return;

		if (m_IsMuted)
		{
			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					music->setVolume(m_PreviousVolume);
				}
			}
			else
			{
				m_SoundEffectPlayer.setVolume(m_PreviousVolume);
			}

			m_IsMuted = false;
		}
		else
		{
			if (m_Clip->m_IsStreaming)
			{
				auto music = static_cast<MusicClip*>(m_Clip->m_Clip.get());
				if (music != nullptr)
				{
					m_PreviousVolume = music->getVolume();
					music->setVolume(0.0f);
				}
			}
			else
			{
				m_PreviousVolume = m_SoundEffectPlayer.getVolume();
				m_SoundEffectPlayer.setVolume(0.0f);
			}

			m_IsMuted = true;
		}
	}

} // Maize