#pragma once

#include "Maize/Debug/Log.h"
#include "Maize/Core/Application.h"

int main()
{
	Maize::Log::Initialize();

    auto app = Maize::CreateApplication();
    app->Run();
}