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
			tx = Texture::Create("/home/finley/GameShiz/Sprites/PNG/Retina/star_large.png");
			auto sprite = Sprite(*tx, { 0, 0, 128, 128 }, { 128.0f / 2, 128.0f / 2});

			auto camera = m_Scene.CreateEntity();
			camera.AddComponent<Camera>();

			a = m_Scene.CreateEntity();
			a.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationA = a.GetComponent<Relationship>();

			Entity b = m_Scene.CreateEntity();
			b.GetComponent<LocalTransform>().position.x = 1.5;
			b.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationB = b.GetComponent<Relationship>();

			relationB.parent = a;
			relationA.firstChild = b;

			Entity c = m_Scene.CreateEntity();
			c.GetComponent<LocalTransform>().position.y = 0.5;
			c.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationC = c.GetComponent<Relationship>();

			relationC.parent = b;
			relationB.firstChild = c;

			Entity d = m_Scene.CreateEntity();
			d.GetComponent<LocalTransform>().position.y = 0.5;
			d.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& relationD = d.GetComponent<Relationship>();

			relationD.parent = c;
			relationC.firstChild = d;

			m_Scene.Initialize();
		}

		void OnDetach() override
		{
			m_Scene.Shutdown();
		}

		void OnUpdate(float deltaTime) override
		{
			auto& t = a.GetComponent<Transform>();

			if (Input::IsKeyPressed(KeyCode::W)) t.position.y += 5 * deltaTime;
			if (Input::IsKeyPressed(KeyCode::S)) t.position.y -= 5 * deltaTime;
			if (Input::IsKeyPressed(KeyCode::D)) t.position.x += 5 * deltaTime;
			if (Input::IsKeyPressed(KeyCode::A)) t.position.x -= 5 * deltaTime;

			m_Scene.Run(deltaTime);
		}

	private:
		Entity a;

		Scene m_Scene;

		std::shared_ptr<Texture> tx;
	};

}
