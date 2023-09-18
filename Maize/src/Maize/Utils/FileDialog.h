#pragma once

#include <portable-file-dialogs.h>

#include <string>
#include <vector>

namespace Maize {

    struct FileFilter
    {
        std::string description = "All Files";
        std::string extensions = "*";
    };

    class FileDialog
    {
    public:
        static std::string OpenFile(const std::vector<FileFilter>& filters = {{}});
        static std::string SaveFile(const std::vector<FileFilter>& filters = {{}});
    };

} // Maize