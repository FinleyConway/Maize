#include <Maize.h>
#include <Maize/Core/EntryPoint.h>

#include "Sandbox.h"

namespace Maize {

	class MaizeOil : public Application
	{
	public:
		MaizeOil(const ApplicationSpecification& specification) : Application(specification)
		{
			PushLayer(new Sandbox());
		}
	};

	Application* CreateApplication()
	{
		ApplicationSpecification specification;
		specification.name = "MaizeOil";

		return new MaizeOil(specification);
	}

}