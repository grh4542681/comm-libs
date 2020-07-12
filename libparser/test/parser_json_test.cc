#include "parser_json.h"
#include <vector>
#include <map>


int main()
{

    printf("---------------\n");
    parser::ParserJson pj;
    printf("---------------\n");
    pj.ParserJsonFile("/home/ezgaoro/workspace/cppfram/etc/pm.json");
    char data[200];
    int idata;
    memset(data, 0x00, sizeof(data));
    //pj.getString("/order/0/name", data, 200);
    pj.find("/order/1").Vfind("/name").getString(data, 200);
    printf("%s\n", data);
    pj.find("/order/0").Vfind("/name").getString(data, 200);
    printf("%s\n", data);
    printf("---------------\n");
    std::vector<parser::ParserJsonObject> v;
    pj.find("/order").getArray(&v);
    for (auto& obj : v) {
        std::map<std::string, parser::ParserJsonObject> m;
        obj.getObject(&m);
        m["name"].getString(data, 200);
        printf("%s\n", data);
        obj.Vfind("/overtime").getInt(&idata);
        printf("%d\n", idata);
    }
    printf("---------------\n");
    pj.find("/order")[0]["name"].getString(data, 200);
    printf("%s\n", data);

    pj.find("/root").objectAdd("testobj", parser::JsonType::OBJECT)["testobj"].objectAdd("teststring", "hello", 4).objectAdd("testint", 200).objectAdd("testbool", false).objectAdd("testdouble", 12.12).objectAdd("123",parser::JsonType::INT);
    pj.find("/").objectAdd("testobj2", parser::JsonType::OBJECT)["testobj2"].objectAdd("testarray", parser::JsonType::ARRAY)["testarray"].arrayPush(1).arrayPush(2).arrayPush(3).arrayPush("4");
    //pj.find("/testobj2").objectAdd("testarray2", parser::JsonType::ARRAY)["testarray2"].arrayPush("1",1).arrayPush("2",1).arrayPush("3",1).arrayPush(4);
    pj.find("/testobj2").objectAdd("testarray2", parser::JsonType::ARRAY)["testarray2"].arrayPush("1").arrayPush("2").arrayPush("3").arrayPush(4);
    pj.find("/testobj2").objectAdd("testarray3", parser::JsonType::ARRAY)["testarray3"].arrayPush(parser::JsonType::OBJECT,3)[1].objectAdd("id",1).objectAdd("name","zhang san");
    pj.StorageJsonFile("test_json1.json");
    pj.find("/testobj2/testarray2").arrayClear();
    pj.find("/testobj2/testarray3")[1].objectClear();
    pj.StorageJsonFile("test_json2.json");

    parser::ParserJson o1;
    parser::ParserJson o2;
    o2.find("/").setObject().objectAdd("sid1",1);
    o1.find("/").setObject().objectAdd("id1",1).objectAdd("o2",o2.root);
    pj.find("/testobj2/testarray3").arrayPush(o1.find("/")).arrayPush(o2.find("/"));
    pj.StorageJsonFile("test_json3.json");
    std::string str1;
    pj.StorageJsonString(str1);
    printf("%s\n",str1.c_str());

    return 0;
}
