#pragma once

#include "Maize/Core/Layer.h"
#include "Maize/Scene/SceneManager.h"
#include "Maize/Core/Input.h"
#include "Maize/Core/KeyCodes.h"

namespace Maize {

	class Window;
	class Renderer;

	class GameLayer : public Layer
	{
	 public:
		GameLayer(Window& window, Renderer& renderer, Input& input);

		void OnEvent(Maize::Event &event) override;
		void OnUpdate(float deltaTime) override;

	 private:
		Window& m_Window;
		Renderer& m_Renderer;
		Input& m_Input;

		SceneManager m_SceneManager;

		float m_FixedAccumulatedTime = 0;
		static constexpr float m_FixedTimeStep = 1.0f / 60.0f;
	};

} // Maize