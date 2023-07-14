#pragma once

#include <algorithm>
#include <unordered_map>

#include "Core/Point.h"
#include "Core/Rect.h"
#include "Core/Renderer.h"
#include "Scene/Components.h"
#include "Scene/Systems/ISystem.h"

namespace Maize {

	class RenderingSystem : public ISystem
	{
	public:
		RenderingSystem(Renderer& renderer);

		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
		void OnRender(ECS::EntityWorld& registry, float dt) override;

	private:
		Renderer& m_Renderer;

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
		void GetSpriteRenderData(ECS::EntityWorld& registry, const CameraData& cameraData, std::unordered_map<Texture*, std::vector<SpriteRenderData>>& spriteBatches);
		bool IsVisibleInCamera(const SpriteRenderData& renderData, const CameraData& cameraData) const;
		void SortSpriteBatches(std::unordered_map<Texture*, std::vector<SpriteRenderData>>& spriteBatches) const;
		SDL_RendererFlip FlipSprite(const SpriteComponent& sprite) const;
		void RenderSprite(const SpriteRenderData& renderData, const CameraData& cameraData);
	};

}