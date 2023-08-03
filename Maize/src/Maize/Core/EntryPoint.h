#pragma once

#include "Maize/Core/Application.h"

extern Maize::Application* Maize::CreateApplication();

int main(int argc, char* argv[])
{
	auto app = Maize::CreateApplication();

	app->Run();

	delete app;

	return 0;
}