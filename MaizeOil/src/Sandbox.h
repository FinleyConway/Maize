#pragma once

#include <Maize.h>

using namespace Maize;

class Sandbox : public Maize::Layer
{
public:
	void OnRender() override
	{
		ImGui::ShowDemoWindow();
	}
};