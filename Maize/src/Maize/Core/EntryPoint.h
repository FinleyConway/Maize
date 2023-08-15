#pragma once

#include "Maize/Core/Application.h"

extern Maize::Application* Maize::CreateApplication();

int main()
{
	auto app = Maize::CreateApplication();

	app->Run();

	delete app;
}