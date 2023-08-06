#pragma once

#include <Maize.h>

using namespace Maize;

class Sandbox : public Maize::Layer
{
public:
	void OnUpdate(float deltaTime) override
	{
		PointF direction;

		if (Input::GetButton(KeyCode::W))
		{
			direction.y = 1;
		}
		if (Input::GetButton(KeyCode::S))
		{
			direction.y = -1;
		}
		if (Input::GetButton(KeyCode::A))
		{
			direction.x = 1;
		}
		if (Input::GetButton(KeyCode::D))
		{
			direction.x = -1;
		}

		std::cout << direction.x << " " << direction.y << std::endl;
	}

	void OnRender() override
	{
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
	}
};