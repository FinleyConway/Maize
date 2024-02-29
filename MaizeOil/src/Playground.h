#pragma once

#include <Maize.h>

namespace Maize {

	void SetParentChildRelationship(entt::registry& registry, entt::entity parent, std::vector<entt::entity>& children)
	{
		if (children.empty())
		{
			return;
		}

		// Set the parent of each child entity
		for (auto child : children)
		{
			registry.get<Relationship>(child).parent = parent;
		}

		// Set the first child of the parent entity
		registry.get<Relationship>(parent).firstChild = children.front();

		// Set the prev and next members for each child entity
		for (size_t i = 0; i < children.size(); ++i)
		{
			auto child = children[i];
			auto& childRelation = registry.get<Relationship>(child);

			if (i > 0)
			{
				childRelation.prev = children[i - 1];
			}

			if (i < children.size() - 1)
			{
				childRelation.next = children[i + 1];
			}
		}
	}

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			tx = Texture::Create("/home/finley/GameShiz/Sprites/PNG/Retina/star_small.png");
			auto sprite = Sprite(*tx, { 0, 0, 128, 128 }, { 128.0f / 2, 128.0f / 2});

			sun = m_Scene.CreateEntity();
			sun.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& sunR = sun.GetComponent<Relationship>();

			planet1 = m_Scene.CreateEntity();
			planet1.GetComponent<LocalTransform>().scale = Vector2(0.75, 0.75);
			planet1.GetComponent<LocalTransform>().position.x = 1;
			planet1.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationPlanet1 = planet1.GetComponent<Relationship>();

			sunR.firstChild = planet1;

			planet2 = m_Scene.CreateEntity();
			planet2.GetComponent<LocalTransform>().scale = Vector2(0.5, 0.5);
			planet2.GetComponent<LocalTransform>().position.x = 0.2;
			planet2.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationPlanet2 = planet2.GetComponent<Relationship>();

			relationPlanet2.parent = planet1;
			relationPlanet1.firstChild = planet2;

			m_Scene.Initialize();
		}

		void OnDetach() override
		{
			m_Scene.Shutdown();
		}

		void OnUpdate(float deltaTime) override
		{
			auto& sun1 = sun.GetComponent<Transform>();
			auto& t1 = planet1.GetComponent<LocalTransform>();
			auto& t2 = planet2.GetComponent<LocalTransform>();

			if (Input::IsKeyPressed(KeyCode::A)) sun1.position.x -= 5 * deltaTime;
			if (Input::IsKeyPressed(KeyCode::D)) sun1.position.x += 5 * deltaTime;

			sun1.angle += 100 * deltaTime;
			t1.angle += 50 * deltaTime;
			t2.angle += 20 * deltaTime;

			m_Scene.Run(deltaTime);
		}

	private:
		Entity sun;
		Entity planet1;
		Entity planet2;

		Scene m_Scene;

		std::shared_ptr<Texture> tx;
	};

}
