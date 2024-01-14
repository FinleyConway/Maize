#pragma once

#include <Maize.h>

namespace Maize {

	class Playground : public Layer
	{
	public:
		Playground() :
		m_MusicClip("/home/finley/Downloads/719098__gregorquendel__julian-eltinge-tango-percy-wenrich-arranged-for-piano.wav"),
		m_FxClip("/home/finley/Downloads/Pew.wav")
		{
		}

		void OnAttach() override
		{
			m_Fx.SetClip(m_FxClip);
			m_Music.SetClip(m_MusicClip);

			auto camera = m_Scene.CreateEntity();
			camera.AddComponent<CameraComponent>();

			auto entity = m_Scene.CreateEntity();
			entity.AddComponent<RigidbodyComponent>().type = BodyType::Dynamic;
			entity.AddComponent<BoxColliderComponent>().restitution = 1.f;

			auto entity1 = m_Scene.CreateEntity();
			entity1.GetComponent<TransformComponent>().position.y = -1.5f;
			entity1.AddComponent<RigidbodyComponent>();
			entity1.AddComponent<BoxColliderComponent>();

			RenderingSystem::drawDebug = true;

			m_Scene.Initialize();
		}

		void OnDetach() override
		{
			m_Scene.Shutdown();
		}

		void OnUpdate(float deltaTime) override
		{
			m_Scene.Run(deltaTime);

			if (ImGui::Button("Play Music")) m_Music.Play();
			if (ImGui::Button("Pause Music")) m_Music.Pause();
			if (ImGui::Button("Mute Music")) m_Music.ToggleMute();
			if (ImGui::Button("Stop Music")) m_Music.Stop();

			static float volume = 0;
			static float pitch = 0;
			if (ImGui::SliderFloat("Volume", &volume, 0, 100)) m_Music.SetVolume(volume);
			if (ImGui::SliderFloat("Pitch", &pitch, 0, 100)) m_Music.SetPitch(pitch);

			if(ImGui::Button("Pew"))
			{
				float random = Random::Range(0.25f, 1.75f);
				m_Fx.SetPitch(random);
				m_Fx.Play();
			}

			if (ImGui::Button("Un-/ToggleMute Pew"))
			{
				m_Fx.ToggleMute();
			}
		}

	private:
		Scene m_Scene;

		AudioClip m_MusicClip;
		AudioClip m_FxClip;
		AudioSource m_Fx;
		AudioSource m_Music;
	};

}
