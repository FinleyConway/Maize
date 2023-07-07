#pragma once

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

		virtual void OnUpdate(ECS::EntityWorld& registry, float dt) override;
		virtual void OnRender(ECS::EntityWorld& registry, float dt) override;

	private:
		struct CameraGroup
		{
			const TransformComponent& transform;
			const CameraComponent& camera;
		};

		struct SpriteGroup
		{
			const TransformComponent& transform;
			const Texture& texture;
		};

		Renderer& m_Renderer;
		AssetManager& m_AssetManager;

		void CreateBatches(ECS::EntityWorld& registry, auto& batch);
		void RenderSprite(const SpriteGroup& spriteGroup, const CameraGroup& cameraGroup);
	};

}