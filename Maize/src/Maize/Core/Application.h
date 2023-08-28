#pragma once

#include <string>

int main();

namespace Maize {

    struct ApplicationSpecification
    {
        std::string name = "Maize";
    };

    class Application
    {
    public:
        explicit Application(const ApplicationSpecification& specification);

    private:
        friend int ::main();

        void Run();

    private:
        bool m_IsRunning = true;
        bool m_Minimized = false;
    };

    Application* CreateApplication();

} // Maize