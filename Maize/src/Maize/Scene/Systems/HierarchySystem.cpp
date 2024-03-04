#include "mpch.h"
#include "Maize/Scene/Systems/HierarchySystem.h"

#include "Maize/Scene/Components.h"

namespace Maize {

	void HierarchySystem::Update(entt::registry& registry, float deltaTime)
	{
		auto view = registry.view<Transform, Relationship>();
		for (auto entity : view)
		{
			auto& relation = view.get<Relationship>(entity);
			auto& transform = view.get<Transform>(entity);

			// check if this entity is the root entity
			if (relation.parent == entt::null)
			{
				UpdateEntityPositions(registry, entity, transform);
			}
		}
	}

	void HierarchySystem::UpdateEntityPositions(entt::registry& registry, entt::entity entity, const Maize::Transform& parentTransform)
	{
		// set the child position relative to parents position
		auto& transform = registry.get<Transform>(entity);
		auto& localTransform = registry.get<LocalTransform>(entity);
		auto& relation = registry.get<Relationship>(entity);

		transform.position = parentTransform.position + Vector2::Rotate(localTransform.position, parentTransform.angle);
		transform.angle = parentTransform.angle + localTransform.angle;
		transform.scale = Vector2(parentTransform.scale.x * localTransform.scale.x, parentTransform.scale.y * localTransform.scale.y);

		// recursively loop though the children's grand children and so on...
		entt::entity childEntity = relation.firstChild;

		while (childEntity != entt::null)
		{
			UpdateEntityPositions(registry, childEntity, transform);

			auto& childRelation = registry.get<Relationship>(childEntity);
			childEntity = childRelation.next;
		}
	}

} // Maize