#include "mpch.h"
#include "FileDialog.h"

namespace Maize {

    std::string FileDialog::OpenFile(const std::vector<FileFilter> &filters)
    {
        std::vector<std::string> filterString;

        for (const auto& filter : filters)
        {
            filterString.push_back(filter.description);
            filterString.push_back(filter.extensions);
        }

        auto file = pfd::open_file("Select a file", ".", filterString, pfd::opt::none).result();

        if (!file.empty())
        {
            return file[0];
        }

        return std::string();
    }

    std::string FileDialog::SaveFile(const std::vector<FileFilter> &filters)
    {
        std::vector<std::string> filterString;

        for (const auto& filter : filters)
        {
            filterString.push_back(filter.description);
            filterString.push_back(filter.extensions);
        }

        auto file = pfd::open_file("Select a file", ".", filterString, pfd::opt::force_overwrite).result();

        if (!file.empty())
        {
            return file[0];
        }

        return std::string();
    }

} // Maize