#include "Maize/Core/AnimationClip.h"

namespace Maize {

	void AnimationClip::AddFrame(uint32_t index, const Sprite* sprite, uint32_t frameDuration)
	{
		if (index <= m_Clip.size())
		{
			AnimationFrame frame(sprite, frameDuration);
			m_Clip.insert(m_Clip.begin() + index, frame);
		}
	}

	void AnimationClip::RemoveFrame(uint32_t index)
	{
		if (index < m_Clip.size())
		{
			m_Clip.erase(m_Clip.begin() + index);
		}
	}

}