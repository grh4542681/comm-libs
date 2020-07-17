#ifndef __FILE_API_H__
#define __FILE_API_H__

#include "file_return.h"

namespace infra {

FileReturn GetFileName(int fd, std::string& filename);
FileReturn GetFileName(FILE* ffd, std::string& filename);
std::string GetFileExtension(std::string filename);
bool IsExist(std::string filename);

}

#endif
