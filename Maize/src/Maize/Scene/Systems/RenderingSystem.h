#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Core/Application.h"
#include "Maize/Scene/Components.h"

namespace Maize {

	using RenderLayer = std::string;
	using SpriteData = std::vector<std::pair<int32_t, const sf::Sprite*>>;
	using SpriteBatches = std::unordered_map<RenderLayer, SpriteData>;

	class RenderingSystem
	{
	public:
		RenderingSystem() : m_Renderer(Application::Get().GetRenderer()) { }

		void OnRender(ECS::EntityWorld& registry);

	private:
		void RenderSprites(ECS::EntityWorld& registry);
		void GetDrawables(ECS::EntityWorld& registry, SpriteBatches& batches) const;
		void SortDrawables(SpriteBatches& batches);
		void RenderSprite(SpriteBatches& batches);

	private:
		Renderer& m_Renderer;
	};

} // Maize