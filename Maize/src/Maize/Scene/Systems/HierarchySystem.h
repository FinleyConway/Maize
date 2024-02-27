#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/Components.h"

namespace Maize {

	class HierarchySystem
	{
	 public:
		static void Update(entt::registry& registry)
		{
			UpdateScene(registry);
		}

	 private:
		static void UpdateScene(entt::registry& registry)
		{
			auto view = registry.view<Transform, Relationship>();
			for (auto entity : view)
			{
				auto& relation = view.get<Relationship>(entity);
				auto& transform = view.get<Transform>(entity);

				// check if this entity is the root entity
				if (relation.parent == entt::null)
				{
					UpdateEntityPositions(registry, entity, transform.position);
				}
			}
		}

		static void UpdateEntityPositions(entt::registry& registry, entt::entity entity, Vector2 parentPosition)
		{
			// set the child position relative to parents position
			auto& transform = registry.get<Transform>(entity);
			auto& localTransform = registry.get<LocalTransform>(entity);
			auto& relation = registry.get<Relationship>(entity);

			transform.position = parentPosition + localTransform.position;

			// recursively loop though the children's grand children and so on...
			entt::entity childEntity = relation.firstChild;

			while (childEntity != entt::null)
			{
				UpdateEntityPositions(registry, childEntity, transform.position);

				auto& childRelation = registry.get<Relationship>(childEntity);
				childEntity = childRelation.next;
			}
		}
	};

} // Maize
