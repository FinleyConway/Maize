#include <Maize.h>
#include <Maize/Core/EntryPoint.h>

namespace Maize {

	class MaizeOil : public Application
	{

	};

	Application* CreateApplication()
	{
		return new MaizeOil();
	}

}