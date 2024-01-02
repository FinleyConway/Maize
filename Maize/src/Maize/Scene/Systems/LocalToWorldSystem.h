#pragma once

#include "EntityComponentSystem/EntityWorld.h"

#include "Maize/Scene/Components.h"

namespace Maize {

	class LocalToWorldSystem
	{
	public:
		void OnUpdate(ECS::EntityWorld& reg, float deltaTime)
		{
			for (auto parentEntity : reg.GetEntityGroup<TransformComponent, ParentComponent>())
			{
				const auto& [parentTransform, parent] = reg.GetComponents<TransformComponent, ParentComponent>(parentEntity);

				// recursively update transforms for children
				UpdateChildTransforms(reg, parentEntity, parentTransform, deltaTime);
			}
		}

	private:
		void UpdateChildTransforms(ECS::EntityWorld& reg, ECS::Entity parentEntity, const TransformComponent& parentTransform, float deltaTime)
		{
			for (auto childEntity : reg.GetEntityGroup<ChildComponent>())
			{
				const auto& [childTransform, childLocalTransform, childParent] = reg.GetComponents<TransformComponent, LocalTransformComponent, ChildComponent>(childEntity);

				// check if this childEntity is a child of the current parentEntity
				if (childParent.parent == parentEntity)
				{
					// inherit transformations
					childTransform.position = parentTransform.position + childLocalTransform.position;
					childTransform.angle = parentTransform.angle + childLocalTransform.angle;
					childTransform.scale.x = parentTransform.scale.x * childLocalTransform.scale.x;
					childTransform.scale.y = parentTransform.scale.y * childLocalTransform.scale.y;

					// recursively update transforms for grandchildren
					UpdateChildTransforms(reg, childEntity, childTransform, deltaTime);
				}
			}
		}
	};

} // Maize
