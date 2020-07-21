#include "file.h"
#include "file_fd.h"
#include "file_api.h"

int main()
{
    infra::file::File cf("test.txt");
    infra::file::File cf1(std::forward<infra::file::FD>(stdout));

    cf1.Open(infra::file::File::Mode::APPEND|infra::file::File::Mode::READ_ONLY|infra::file::File::Mode::WRITE_ONLY);
    cf1.Open(infra::file::File::Mode::APPEND);
    cf1.GetFileFD().Write("hello\n", 6);

    std::string name;
    infra::file::GetFileName(stdout, name);
    printf("name %s\n",name.c_str());

    printf("name %s desc %s \n",cf.GetFileName().c_str(),cf.GetFileFormatDescribe().c_str());
    printf("name %s desc %s \n",cf1.GetFileName().c_str(),cf1.GetFileFormatDescribe().c_str());

    return 0;
}
