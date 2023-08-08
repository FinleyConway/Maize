#pragma once

#include <Maize.h>

using namespace Maize;

class Sandbox : public Maize::Layer
{
public:
	void OnUpdate(float deltaTime) override
	{
		std::cout << deltaTime << std::endl;
	}

	void OnRender() override
	{
		//bool show_demo_window = true;
		//ImGui::ShowDemoWindow(&show_demo_window);
	}
};