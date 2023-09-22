#pragma once

#include <portable-file-dialogs.h>

namespace Maize {

    enum class NotifyNote { Info, Warning, Error };

    class SystemNotify
    {
    public:
        static void Notify(const std::string& title, const std::string& message, NotifyNote icon);
    };

} // Maize