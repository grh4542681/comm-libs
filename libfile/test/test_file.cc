#include "file.h"
#include "file_fd.h"
#include "file_api.h"

int main()
{
    file::File cf("test.txt");
    file::File cf1(std::forward<file::FileFD>(stdout));

    cf1.Open(file::File::Mode::APPEND|file::File::Mode::READ_ONLY|file::File::Mode::WRITE_ONLY);
    cf1.Open(file::File::Mode::APPEND);
    cf1.GetFileFD().Write("hello\n", 6);

    std::string name;
    file::GetFileName(stdout, name);
    printf("name %s\n",name.c_str());

    printf("name %s desc %s \n",cf.GetFileName().c_str(),cf.GetFileFormatDescribe().c_str());
    printf("name %s desc %s \n",cf1.GetFileName().c_str(),cf1.GetFileFormatDescribe().c_str());

    return 0;
}
