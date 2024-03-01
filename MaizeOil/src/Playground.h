#pragma once

#include <Maize.h>

namespace Maize {

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			tx = Texture("/home/finley/Downloads/A-Aron Head.png");
			auto sprite = Sprite(tx, { 0, 0, 988, 1287 }, { 988.0f / 2, 1287.0f / 2});

			sun = m_Scene.CreateEntity();
            sun.GetComponent<Transform>().scale = Vector2(0.25, 0.25);
			sun.AddComponent<SpriteRenderer>().sprite = sprite;
			auto& sunR = sun.GetComponent<Relationship>();

			planet1 = m_Scene.CreateEntity();
			planet1.GetComponent<LocalTransform>().scale = Vector2(0.75, 0.75);
			planet1.GetComponent<LocalTransform>().position.x = 2;
			auto& p1 = planet1.AddComponent<SpriteRenderer>();
            p1.sprite = sprite;
            p1.orderInLayer = 1;
			auto& relationPlanet1 = planet1.GetComponent<Relationship>();

			sunR.firstChild = planet1;

			planet2 = m_Scene.CreateEntity();
			planet2.GetComponent<LocalTransform>().scale = Vector2(0.5, 0.5);
			planet2.GetComponent<LocalTransform>().position.x = 1;
			auto& s = planet2.AddComponent<SpriteRenderer>();
            s.sprite = sprite;
            s.orderInLayer = 2;
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

		Texture tx;
	};

}
