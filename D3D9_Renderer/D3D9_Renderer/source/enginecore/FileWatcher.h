#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace renderer
{
    class FileWatcher
    {
    public:
        FileWatcher();
        ~FileWatcher();

        [[nodiscard]]size_t AddFileForWatch(std::string filePath);
        bool IsFileModified(size_t fileIndex);
    private:
        std::vector<std::string> m_filePathToWatch;
        std::vector<std::chrono::system_clock::time_point> m_watchFilesLastWriteTime;
    };
}