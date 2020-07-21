#include "file_path.h"

int main() {
    std::string s = "  a/a/a/a/a/a/a/a/a/a/opt///////test//11 txt ";
    infra::file::Path p(s);
    printf("%s\n",p.GetRaw().c_str());
    printf("%s\n",p.GetPath().c_str());

    infra::file::Path p1("/1/2/3/");
    infra::file::Path p2("/4/5/6");
    p1 << p2;
    printf("%s\n",p1.GetPath().c_str());
    p1 << "5/12/3/wq";
    printf("%s\n",p1.GetPath().c_str());
    return 0;
}
