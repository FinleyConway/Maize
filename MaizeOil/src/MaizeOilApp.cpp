#include <Maize/Core/EntryPoint.h>

#include "Playground.h"

namespace Maize {

    class MaizeOil : public Application
    {
    public:
        explicit MaizeOil(const ApplicationSpecification& specification) : Application(specification)
        {
			PushLayer(new Playground);
        }
    };

    std::unique_ptr<Application> CreateApplication()
    {
        ApplicationSpecification specification;
        specification.name = "MaizeOil";

        return std::make_unique<MaizeOil>(specification);
    }

} // Maize