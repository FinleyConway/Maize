#include "mpch.h"
#include "Maize/Audio/AudioSource.h"

namespace Maize
{
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

	void AudioSource::Pause()
	{
		if (m_Clip == nullptr) return;

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
		if (m_Clip != nullptr) return;

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