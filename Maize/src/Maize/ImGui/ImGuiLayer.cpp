#include "Maize/ImGui/ImGuiLayer.h"

#include "Maize/Core/Application.h"

namespace Maize {

	void ImGuiLayer::OnAttach()
	{
		Application& app = Application::Get();
		ImGui::SFML::Init(app.GetWindow());
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui::SFML::Shutdown();
	}

	void ImGuiLayer::Begin(sf::Time time) const
	{
		Application& app = Application::Get();
		ImGui::SFML::Update(app.GetWindow(), time);
	}

	void ImGuiLayer::End() const
	{
		Application& app = Application::Get();
		ImGui::SFML::Render(app.GetWindow());
	}

}