#pragma once

#include <SDL.h>
#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Math/Point.h"
#include "Math/Rect.h"
#include "Renderer/Renderer.h"
#include "Scene/Components.h"
#include "System.h"

namespace Maize {

	class RenderingSystem : public System
	{
	public:
		void OnRender(ECS::EntityWorld& registry, float dt) override;

	private:
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