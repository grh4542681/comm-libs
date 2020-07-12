#include "string_util.h"

#include "file_path.h"

namespace file {

FilePath::FilePath()
{
    raw_.clear();
    path_vector_.clear();
    abs_path_ = false;
}

FilePath::FilePath(std::string raw) : raw_(raw)
{
    path_vector_.clear();
    abs_path_ = false;
    if (raw_.empty()) {
        return;
    } else {
        util::String::Trim(raw_);
        util::String::Distinct(raw_, '/');
        if (raw_[0] == '/') {
            abs_path_ = true;
            raw_.erase(0, 1);
        } else {
            if (raw_[0] != '.' && raw_[1] != '/') {
                raw_ = "./" + raw_;
            }
        }
        util::String::Split(raw_, "/", path_vector_);
        depth_ = path_vector_.size() - 1;
    }
}

FilePath::~FilePath() {}

std::string& FilePath::GetRaw()
{
    return raw_;
}

FilePath& FilePath::operator<<(FilePath& other)
{
    path_vector_.insert(path_vector_.end(), other.path_vector_.begin(), other.path_vector_.end());
    depth_ = path_vector_.size() - 1;
    return *this;
}

FilePath& FilePath::operator<<(std::string&& other)
{
    FilePath p(other);
    return (*this << p);
}

FilePath& FilePath::operator<<(std::string& other)
{
    return FilePath::operator<<(std::move(other));
}

int FilePath::GetDepth()
{
    return depth_;
}

std::string FilePath::GetPath()
{
    return GetPath(0, depth_);
}

std::string FilePath::GetPath(unsigned int depth) {
    return GetPath(depth, depth);
}

std::string FilePath::GetPath(unsigned int start, unsigned int depth) {
    std::string path;
    path.clear();
    if (start <= depth && depth <= depth_) {
        auto loop = start;
        for ( ; loop <= depth; loop++) {
            path = path + "/" + path_vector_[loop];
        }
    }
    if (!abs_path_ || start != 0) {
        path.erase(0, 1);
    }
    return path;
}

}
