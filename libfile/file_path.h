#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

#include <string>
#include <vector>

namespace infra::file {

class Path {
public:
    Path();
    Path(std::string raw);
    ~Path();

    Path& operator<<(std::string& other);
    Path& operator<<(std::string&& other);
    Path& operator<<(Path& other);

    std::string& GetRaw();
    int GetDepth();
    std::string GetPath();
    std::string GetPath(unsigned int depth);
    std::string GetPath(unsigned int start, unsigned int depth);

private:
    std::string raw_;
    unsigned int depth_;
    bool abs_path_;
    std::vector<std::string> path_vector_;
};

}

#endif
