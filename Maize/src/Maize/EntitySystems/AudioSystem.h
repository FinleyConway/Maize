#pragma once

#include <SDL_mixer.h>
#include <algorithm>
#include <cmath>

#include "Maize/EntitySystems/System.h"
#include "Maize/Scene/Components.h"
#include "Maize/Core/SoundClip.h"
#include "Maize/Math/Point.h"

namespace Maize {

	class AudioSystem : public System
	{
	public:
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;

	private:
		int32_t GetFreeChannel() const;
		void PlayAudioClips(AudioSourceComponent& audio, const TransformComponent& listener, const TransformComponent& source) const;
		void PlayPositionalSound(const TransformComponent& listener, const TransformComponent& source, const AudioSourceComponent& audio, const SoundClip* clip) const;
		void PlaySound(const AudioSourceComponent& audio, const SoundClip* clip) const;
		float CalculateAttenuation(float distance, float minDistance, float maxDistance) const;

	};

}