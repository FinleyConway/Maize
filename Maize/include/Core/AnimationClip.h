#pragma once

#include <string>
#include <vector>

#include "Renderer/Sprite.h"

namespace Maize {

	struct AnimationFrame
	{
		const Sprite* sprite = nullptr;
		uint32_t frameDuration = 0;
		float elapsedTime = 0;

		AnimationFrame(const Sprite* sprite, uint32_t frameDuration) : sprite(sprite), frameDuration(frameDuration) { }
	};

	struct AnimationClip
	{
	public:
		const std::vector<AnimationFrame>& Clip() const { return m_Clip; }

		void AddFrame(uint32_t index, const Sprite* sprite, uint32_t frameDuration);
		AnimationFrame& GetFrame(uint32_t index) { return m_Clip[index]; }
		void RemoveFrame(uint32_t index);

	private:
		std::vector<AnimationFrame> m_Clip;
	};

}