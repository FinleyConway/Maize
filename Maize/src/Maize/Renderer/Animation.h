#pragma once

#include "Maize/Renderer/Sprite.h"

namespace Maize {

	class Animation
	{
	public:
		void AddFrame(uint32_t index, const sf::IntRect& rect, float duration);
		bool RemoveFrame(uint32_t index);

		float GetDuration() const { return m_TotalDuration; }
		uint32_t GetCurrentFrame() const { return m_CurrentFrame; }

		void Update(Sprite& sprite, float deltaTime);

	private:
		struct Frame
		{
			sf::IntRect rect;
			float duration = 0.0f;
			float elapsedTime = 0.0f;
		};

		std::vector<Frame> m_Frames;
		uint32_t m_CurrentFrame = 0;
		float m_TotalDuration = 0.0f;
	};

} // Maize