#include "parser_yaml.h"

int main() {

    parser::ParserYaml p;
    p.LoadFile("/home/ezgaoro/workspace/cppfram/plat/etc/process/messaged.yaml");
    auto obj = p.GetDoc().Find("message");
    auto obj1 = p.GetDoc().Find("message").Find("name");
    auto obj2 = p.GetDoc().Find("message").Find("sockfile");
    auto obj3 = p.GetDoc().Find("message").Find("listener_size");

    std::string name;
    obj1.GetData<std::string>(&name);
    printf("name - %s\n", name.c_str());
    obj2.GetData<std::string>(&name);
    printf("sockfile - %s\n", name.c_str());
    int ii;
    obj3.GetData<int>(&ii);
    printf("listener_szie - %d\n", ii);

    auto node = YAML::LoadFile("/home/ezgaoro/workspace/cppfram/plat/etc/process/messaged.yaml");
    auto aa = node["message"];
    auto cc = aa;
    auto bb = cc["name"];
    printf("%s\n",bb.as<std::string>().c_str());
    auto aaa = cc["sockfile"];
    printf("%s\n",aaa.as<std::string>().c_str());
    auto dd = cc["listener_size"];
    printf("%d\n",dd.as<int>());

    return 0;
}
