#include "Scene/Systems/AudioSystem.h"

namespace Maize {

	void AudioSystem::OnUpdate(ECS::EntityWorld& registry, float dt)
	{
		for (const auto listener : registry.GetEntityGroup<TransformComponent, AudioListenerComponent>())
		{
			const auto& [listenerTransform, audioListener] = registry.GetComponents<TransformComponent, AudioListenerComponent>(listener);

			Mix_MasterVolume(audioListener.masterVolume);

			if (!audioListener.pauseAudio)
			{
				for (const auto entity : registry.GetEntityGroup<TransformComponent, AudioSourceComponent>())
				{
					const auto& [sourceTransform, audio] = registry.GetComponents<TransformComponent, AudioSourceComponent>(entity);

					if (!audio.mute && !audio.audioClips.empty())
					{
						PlayAudioClips(audio, listenerTransform, sourceTransform);
					}
				}
			}
		}
	}

	int32_t AudioSystem::GetFreeChannel() const
	{
		int32_t channels = Mix_AllocateChannels(-1);

		for (int32_t i = 0; i < channels; i++)
		{
			if (Mix_Playing(i) != 1) return i;
		}

		return -1;
	}

	void AudioSystem::PlayAudioClips(AudioSourceComponent& audio, const TransformComponent& listener, const TransformComponent& source) const
	{
		auto it = audio.audioClips.begin();
		while (it != audio.audioClips.end())
		{
			if ((*it) != nullptr)
			{
				const SoundClip* clip = (*it);

				if (audio.spatial)
				{
					PlayPositionalSound(listener, source, audio, clip);
				}
				else
				{
					PlaySound(audio, clip);
				}
			}

			// remove the clip from the vector after it has been played
			it = audio.audioClips.erase(it);
		}
	}

	void AudioSystem::PlayPositionalSound(const TransformComponent& listener, const TransformComponent& source, const AudioSourceComponent& audio, const SoundClip* clip) const
	{
		float distanceFromListener = PointF::Distance(listener.position, source.position);
		float attenuationFactor = CalculateAttenuation(distanceFromListener, audio.minDistance, audio.maxDistance);
		float finalVolume = audio.volume * attenuationFactor;

		PointF direction = (source.position - listener.position).Normalized();
		float angle = std::atan2f(direction.y, direction.x) * (180.0f / 3.14f);
		angle -= 90.0f; // make it so 0 is up

		uint8_t leftPan = 255 - static_cast<uint8_t>(angle * 255.0f / 360.0f);
		uint8_t rightPan = static_cast<uint8_t>(angle * 255.0f / 360.0f);

		int32_t channel = GetFreeChannel();

		// play sound at a direction and depth
		Mix_SetPanning(channel, leftPan, rightPan);
		Mix_Volume(channel, static_cast<uint8_t>(finalVolume));
		Mix_PlayChannel(channel, *clip, audio.loop ? -1 : 0);
	}

	void AudioSystem::PlaySound(const AudioSourceComponent& audio, const SoundClip* clip) const
	{
		int32_t channel = GetFreeChannel();

		Mix_VolumeChunk(*clip, audio.volume);
		Mix_PlayChannel(channel, *clip, audio.loop ? -1 : 0);
	}

	float AudioSystem::CalculateAttenuation(float distance, float minDistance, float maxDistance) const
	{
		if (distance <= minDistance)
		{
			// the source is within the minimum distance, so it plays at full volume.
			return 1;
		}
		else if (distance >= maxDistance)
		{
			// the source is beyond the maximum distance, so it's too far to be heard (volume = 0).
			return 0;
		}
		else
		{
			// linearly interpolate the volume between min and max distances.
			return 1 - (distance - minDistance) / (maxDistance - minDistance);
		}
	}

}