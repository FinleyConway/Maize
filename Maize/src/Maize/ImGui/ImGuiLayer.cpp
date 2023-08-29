#include "Maize/ImGui/ImGuiLayer.h"

#include "Maize/Core/Application.h"

namespace Maize {

    void ImGuiLayer::OnAttach()
    {
        Application& app = Application::Get();
        if (!ImGui::SFML::Init(app.GetWindow()))
        {
            std::cout << "ImGui sfml didnt init" << std::endl;
        }

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
        const ImGuiIO& io = ImGui::GetIO();

        e.handled |= io.WantCaptureMouse;
        e.handled |= io.WantCaptureKeyboard;
    }

    void ImGuiLayer::OnDetach()
    {
        ImGui::SFML::Shutdown();
    }

    void ImGuiLayer::Begin(Window& window, float deltaTime) const
    {
        ImGui::SFML::Update(window, sf::seconds(deltaTime));

        ImGui::ShowDemoWindow();
    }

    void ImGuiLayer::End(Window& window) const
    {
        ImGui::SFML::Render(window);
    }

} // Maize