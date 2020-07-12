#include "parser_ini.h"

int main()
{
    parser::ParserIni pi("test.ini");
    pi.Load();
    pi.Print();
    std::string a = pi.getConfig("s9m24", "RemoteBoxIP");
    printf("%s\n", a.c_str());
    pi.Storage("test_output.ini");
    pi.Free();
    return 0;
}
