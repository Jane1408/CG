#include "libchapter2_private.h"
#include "FilesystemUtils.h"
#include <boost/filesystem/operations.hpp>
#include <fstream>
#ifdef _WIN32
#include <sdkddkver.h>
#include <Windows.h>
#endif

namespace fs = boost::filesystem;

namespace
{
const size_t FILE_RESERVE_SIZE = 4096;
const size_t MAX_PATH_SIZE = 4096;

// Возвращает путь к текущему исполняемому файлу,
// .exe на Windows, ELF на Linux, MachO на MacOSX
std::string GetExecutablePath()
{
    char buffer[MAX_PATH_SIZE];
    size_t size = sizeof(buffer);
#if defined(_WIN32)
    size = size_t(::GetModuleFileNameA(nullptr, buffer, DWORD(size)));
#elif defined(__linux__)
    ssize_t result = readlink("/proc/self/exe", buffer, size);
    if (result <= 0)
    {
        throw std::runtime_error("Unexpected error on readlink()");
    }
    size = size_t(result);
#elif defined(__apple__)
    uint32_t sizeU32 = uint32_t(size);
    if (_NSGetExecutablePath(buffer, &sizeU32) != 0)
    {
        throw std::runtime_error("Unexpected error on _NSGetExecutablePath()");
    }
    size = size_t(sizeU32);
#endif
    return std::string(buffer, size_t(size));
}
}

boost::filesystem::path CFilesystemUtils::GetResourceAbspath(const boost::filesystem::path &currentPath)
{
    if (currentPath.is_absolute())
    {
        return currentPath;
    }

    const fs::path executableDir = fs::path(GetExecutablePath()).parent_path();
    const fs::path currentDir = fs::current_path();
    for (const auto &dir : {executableDir, currentDir})
    {
        fs::path abspath = fs::absolute(currentPath, dir);
        if (fs::exists(abspath))
        {
            return abspath;
        }
    }
    throw std::runtime_error("Resource not found: " + currentPath.generic_string());
}

std::string CFilesystemUtils::LoadFileAsString(const boost::filesystem::path &path)
{
    const fs::path abspath = GetResourceAbspath(path);

    std::ifstream input;
    input.open(path.native());
    if (!input.is_open())
    {
        throw std::runtime_error("Cannot open for reading: " + path.generic_string());
    }

    std::string text;
    text.reserve(FILE_RESERVE_SIZE);
    input.exceptions(std::ios::badbit);

    std::string line;
    while (std::getline(input, line))
    {
        text.append(line);
        text.append("\n");
    }

    return text;
}
