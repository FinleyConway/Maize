#pragma once

#include <SDL.h>
#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Maize/EntitySystems/System.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Application.h"
#include "Maize/Scene/Components.h"
#include "Maize/Math/Point.h"
#include "Maize/Math/Rect.h"

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