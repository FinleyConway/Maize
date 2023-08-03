#include <Maize.h>
#include <Maize/Core/EntryPoint.h>

#include "Sandbox.h"

namespace Maize {

	class MaizeOil : public Application
	{
	public:
		MaizeOil()
		{
			PushLayer(new Sandbox());
		}
	};

	Application* CreateApplication()
	{
		return new MaizeOil();
	}

}