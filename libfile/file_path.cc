#include "string_util.h"

#include "file_path.h"

namespace infra::file {

Path::Path()
{
    raw_.clear();
    path_vector_.clear();
    abs_path_ = false;
}

Path::Path(std::string raw) : raw_(raw)
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

Path::~Path() {}

std::string& Path::GetRaw()
{
    return raw_;
}

Path& Path::operator<<(Path& other)
{
    path_vector_.insert(path_vector_.end(), other.path_vector_.begin(), other.path_vector_.end());
    depth_ = path_vector_.size() - 1;
    return *this;
}

Path& Path::operator<<(std::string&& other)
{
    Path p(other);
    return (*this << p);
}

Path& Path::operator<<(std::string& other)
{
    return Path::operator<<(std::move(other));
}

int Path::GetDepth()
{
    return depth_;
}

std::string Path::GetPath()
{
    return GetPath(0, depth_);
}

std::string Path::GetPath(unsigned int depth) {
    return GetPath(depth, depth);
}

std::string Path::GetPath(unsigned int start, unsigned int depth) {
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
