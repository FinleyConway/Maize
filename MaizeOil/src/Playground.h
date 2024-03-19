#pragma once

#include <Maize.h>

namespace Maize {

	struct Movable { };

	class Test : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override
		{
			auto view = registry.view<Rigidbody, Movable>();
			for (auto [entity, rig] : view.each())
			{
				Vector2 dir;

				if (Input::GetButton(KeyCode::Left)) dir.x--;
				if (Input::GetButton(KeyCode::Right)) dir.x++;
				if (Input::GetButton(KeyCode::Up)) dir.y++;
				if (Input::GetButton(KeyCode::Down)) dir.y--;

				rig.body->ApplyLinearImpulseToCenter({dir.x * 5 * deltaTime, dir.y * 5 * deltaTime}, true);
			}
		}
	};

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			m_Tx = Texture("/home/finley/GameShiz/Sprites/PNG/Retina/station_C.png");
			m_Tx1 = Texture("/home/finley/GameShiz/Sprites/PNG/Retina/star_tiny.png");

			// scene 1
			auto scene1 = SceneManager::CreateScene("Default");
			auto initScene1 = [&](Scene& scene)
			{
				scene.AddSystem<Test>("Test");

				auto e1 = scene.CreateEntity();
			  	auto& t1 = e1.GetComponent<Transform>().position = Vector2(0, 0);
				auto& spriteR = e1.AddComponent<SpriteRenderer>();
				spriteR.sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128.f / 2, 128.f / 2}, 128);
				auto& rb = e1.AddComponent<Rigidbody>();
				rb.type = BodyType::Dynamic;
				rb.gravityScale = 0;
				auto& box = e1.AddComponent<BoxCollider>();
				e1.AddTag<Movable>();

			  	auto e2 = scene.CreateEntity();
			  	auto& t2 = e2.GetComponent<Transform>().position = Vector2(1, 0);
			  	auto& spriteR2 = e2.AddComponent<SpriteRenderer>();
			  	spriteR2.sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128.f / 2, 128.f / 2}, 128);
			  	auto& rb2 = e2.AddComponent<Rigidbody>();
			  	rb2.type = BodyType::Dynamic;
			  	rb2.gravityScale = 0;
			  	auto& box2 = e2.AddComponent<BoxCollider>();
			};
			scene1->AddSceneInitializer(initScene1);

			SceneManager::LoadScene(scene1->GetIndex());
		}

	 private:
		Texture m_Tx;
		Texture m_Tx1;
	};

}
