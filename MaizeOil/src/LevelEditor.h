#pragma once

#include <Maize.h>
#include <EntityComponentSystem/EntityWorld.h>

#include "TilemapEditor/TilemapEditorWindow.h"
#include "Maize/Utils/Grids/CartesianGrid.h"

class LevelEditor : public Maize::Layer
{
public:
    LevelEditor()
    {
        auto tilemapE = m_Reg.CreateEntity();
        m_Reg.AddComponent<Maize::TransformComponent>(tilemapE);
        m_TilemapComponent = &m_Reg.AddComponent<Maize::TilemapComponent>(tilemapE); // temp

		m_TilemapEditorWindow.AddComponent(m_TilemapComponent);

		m_SpriteBatch.setPrimitiveType(sf::Quads);

		auto quad = Maize::Renderer::CreateQuad(sf::Vector2f(50, -10), 45, sf::Vector2f(100, 100), sf::Color::Blue);

		for (auto& vertex : quad)
			m_SpriteBatch.append(vertex);
    }

    void OnEvent(Maize::Event& e) override
    {
        m_Camera.OnEvent(e);

        m_TilemapEditorWindow.OnEvent(e);
    }

    void OnUpdate(float deltaTime) override
    {
        ImGui::ShowDemoWindow();

        m_Camera.OnUpdate(deltaTime);

		if (m_TilemapComponent != nullptr)
		{
			m_TilemapEditorWindow.Window();
		}

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

		ren.Draw(m_SpriteBatch);

		if (m_TilemapComponent->tilemapTexture != nullptr)
		{
			for (auto& layer : m_TilemapComponent->tilemapLayers)
			{
				ren.Draw(layer.GetVertexArray(), m_TilemapComponent->tilemapTexture.get());
			}
		}

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent = nullptr;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	sf::VertexArray m_SpriteBatch;
};