#include "mpch.h"
#include "Maize/Renderer/Animation.h"

namespace Maize {

	void Animation::AddFrame(uint32_t index, const sf::IntRect& rect, float duration)
	{
		m_Frames.emplace(m_Frames.begin() + index, rect, duration);
		m_TotalDuration += duration;
	}

	bool Animation::RemoveFrame(uint32_t index)
	{
		if (index < m_Frames.size())
		{
			m_TotalDuration -= m_Frames[index].duration;
			m_Frames.erase(m_Frames.begin() + index);

			return true;
		}

		return false;
	}

	void Animation::Update(Sprite& sprite, float deltaTime)
	{
		if (m_Frames.empty()) return;

		// set frame to 0 if it reaches the end of the animation
		if (m_CurrentFrame >= m_Frames.size())
		{
			m_CurrentFrame = 0;
		}

		Frame& frame = m_Frames[m_CurrentFrame];

		frame.elapsedTime += deltaTime;

		// set the frame to the sprite
		if (frame.elapsedTime >= frame.duration)
		{
			sprite.SetTextureRect(frame.rect);
			m_CurrentFrame++;
			frame.elapsedTime = 0.0f;
		}
	}

} // Maize