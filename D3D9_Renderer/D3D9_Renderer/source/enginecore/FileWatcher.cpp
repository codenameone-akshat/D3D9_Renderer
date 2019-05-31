#include "FileWatcher.h"
#include <cassert>

namespace filesystem = std::experimental::filesystem;
namespace renderer
{
    FileWatcher::FileWatcher()
        :m_filePathToWatch(),
        m_watchFilesLastWriteTime()
    {
    }

    FileWatcher::~FileWatcher()
    {
    }

    size_t FileWatcher::AddFileForWatch(std::string filePath)
    {
        assert(filesystem::exists(filePath));
        
        auto lastModifiedTime = filesystem::last_write_time(filePath);
        
        m_filePathToWatch.emplace_back(filePath);
        m_watchFilesLastWriteTime.emplace_back(lastModifiedTime);
        
        return (m_filePathToWatch.size() - 1); //return the index of the element added
    }

    bool FileWatcher::IsFileModified(size_t fileIndex)
    {
        assert(fileIndex < m_filePathToWatch.size());

        auto time = filesystem::last_write_time(m_filePathToWatch[fileIndex]);
        
        if (time > m_watchFilesLastWriteTime[fileIndex])
        {
            m_watchFilesLastWriteTime[fileIndex] = time;
            return true;
        }
        
        return false;
    }
}