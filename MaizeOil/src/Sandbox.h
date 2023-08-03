#pragma once

#include <Maize.h>

class Sandbox : public Maize::Layer
{
public:
	void OnRender() override
	{
		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);
	}
};