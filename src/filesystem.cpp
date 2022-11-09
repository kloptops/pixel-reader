#include "filesystem.h"

#include <dirent.h>
#include <algorithm>

static std::string to_lower(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::vector<FSEntry> directory_listing(const std::string& path)
{
    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        return {};
    }

    std::vector<FSEntry> filenames;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        bool is_dir = entry->d_type == DT_DIR;
        bool is_file = entry->d_type == DT_REG;
        std::string name = std::string(entry->d_name);
        if ((is_dir || is_file) && name != "." && name != "..")
        {
            filenames.push_back({name, is_dir});
        }
    }

    closedir(dir);

    std::sort(filenames.begin(), filenames.end(), [](const FSEntry& a, const FSEntry& b) {
        if (a.is_dir && !b.is_dir) {
            return true;
        }
        return to_lower(a.name) < to_lower(b.name);
    });
    return filenames;
}
