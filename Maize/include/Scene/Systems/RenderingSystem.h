#pragma once

#include <algorithm>
#include <unordered_map>

#include "Scene/Systems/ISystem.h"
#include "Core/Renderer.h"
#include "Scene/Components.h"
#include "Scene/AssetManager.h"

namespace Maize {

	class RenderingSystem : public ISystem
	{
	public:
		RenderingSystem(Renderer& renderer, AssetManager& asset);

		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
		void OnRender(ECS::EntityWorld& registry, float dt) override;

	private:
		Renderer& m_Renderer;
		AssetManager& m_AssetManager;

		struct CameraData
		{
			const TransformComponent& transform;
			const CameraComponent& camera;
		};

		struct SpriteRenderData
		{
			const TransformComponent* transform;
			const SpriteComponent* sprite;
		};

		void RenderSprites(ECS::EntityWorld& registry, const CameraData& cameraData);
		void GetSpriteRenderData(ECS::EntityWorld& registry, std::unordered_map<std::string, std::vector<SpriteRenderData>>& spriteBatches);
		void SortSpriteBatches(std::unordered_map<std::string, std::vector<SpriteRenderData>>& spriteBatches);
		SDL_RendererFlip FlipSprite(const SpriteComponent& sprite);
		void RenderSprite(const SpriteRenderData& renderData, const CameraData& cameraData, const Texture& texture);
	};

}