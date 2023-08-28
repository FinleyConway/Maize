#include <Maize.h>
#include <Maize/Core/EntryPoint.h>

namespace Maize {

    class MaizeOil : public Application
    {
    public:
        explicit MaizeOil(const ApplicationSpecification& specification) : Application(specification)
        {
        }
    };

    Application* CreateApplication()
    {
        ApplicationSpecification specification;
        specification.name = "MaizeOil";

        return new MaizeOil(specification);
    }

}