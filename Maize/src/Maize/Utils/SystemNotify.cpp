#include "mpch.h"
#include "Maize/Utils/SystemNotify.h"

namespace Maize {

    void SystemNotify::Notify(const std::string &title, const std::string &message, NotifyNote icon)
    {
        pfd::notify(title, message, (pfd::icon)icon);
    }

} // Maize