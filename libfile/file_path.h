#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

#include <string>
#include <vector>

namespace file {

class FilePath {
public:
    FilePath();
    FilePath(std::string raw);
    ~FilePath();

    FilePath& operator<<(std::string& other);
    FilePath& operator<<(std::string&& other);
    FilePath& operator<<(FilePath& other);

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
