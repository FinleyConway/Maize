#include "Maize/Layers/GameLayer.h"

namespace Maize {

	GameLayer::GameLayer(Window& window, Renderer& renderer, Input& input) : m_Window(window), m_Renderer(renderer), m_Input(input)
	{
	}

	void GameLayer::OnEvent(Event& event)
	{
	}

	void GameLayer::OnUpdate(float deltaTime)
	{
		// fixed time step
		m_FixedAccumulatedTime += deltaTime;

		while (m_FixedAccumulatedTime >= m_FixedTimeStep)
		{
			// fixed update
			m_SceneManager.OnFixedUpdate(m_FixedTimeStep);

			m_FixedAccumulatedTime -= m_FixedTimeStep;
		}

		// update
		m_SceneManager.OnUpdate(deltaTime);
	}

} // Maize