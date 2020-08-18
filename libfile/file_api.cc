#include <unistd.h>
#include <vector>

#include "string_util.h"

#include "file_api.h"
#include "file_log.h"

namespace infra::file {

Return GetFileName(int fd, std::string& ofilename)
{
    char buf[1024];
    char filename[1024];
    memset(buf ,0x00, sizeof(buf));
    memset(filename, 0x00, sizeof(filename));

    sprintf(buf, "/proc/self/fd/%d", fd);
    if (readlink(buf, filename, sizeof(filename)-1) < 0) {
        int tmperrno = errno;
        Log::Error("Get file name from file descriptor error");
        return tmperrno;
    }
    ofilename = std::string(filename);
    return Return::SUCCESS;
}

Return GetFileName(FILE* ffd, std::string& filename)
{
    return GetFileName(fileno(ffd), filename);
}

std::string GetFileExtension(std::string filename)
{
    std::vector<std::string> vec;
    util::String::Split(filename, ".", vec);
    return vec.back();
}

bool IsExist(std::string filename)
{
    if (access(filename.c_str(), F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

}
