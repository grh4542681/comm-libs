#include "file.h"
#include "file_fd.h"
#include "file_api.h"

int main()
{
    infra::File cf("test.txt");
    infra::File cf1(std::forward<infra::FileFD>(stdout));

    cf1.Open(infra::File::Mode::APPEND|infra::File::Mode::READ_ONLY|infra::File::Mode::WRITE_ONLY);
    cf1.Open(infra::File::Mode::APPEND);
    cf1.GetFileFD().Write("hello\n", 6);

    std::string name;
    infra::GetFileName(stdout, name);
    printf("name %s\n",name.c_str());

    printf("name %s desc %s \n",cf.GetFileName().c_str(),cf.GetFileFormatDescribe().c_str());
    printf("name %s desc %s \n",cf1.GetFileName().c_str(),cf1.GetFileFormatDescribe().c_str());

    return 0;
}
