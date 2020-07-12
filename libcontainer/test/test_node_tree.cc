#include "node_tree.h"

int main() {

//    container::NodeTree::Element<int> eint1("int1");
//    eint1.SetData(5);
//    container::NodeTree::Element<int> eint2("int2");
//    eint2.SetData(5);
//    container::NodeTree::Element<int> eint3("int3");
//    eint3.SetData(5);
//    container::NodeTree::Element<int> eint4("int4");
//    eint4.SetData(5);
//    container::NodeTree::Element<int> eint5("int5");
//    eint5.SetData(5);
//    container::NodeTree::Element<int> eint6("int6");
//    eint6.SetData(5);
//    container::NodeTree::Element<int> eint7("int7");
//    eint7.SetData(5);
//
//    container::NodeTree::Element<int> eint("int");
//    eint.SetData(5);
//    printf("%d\n",eint.GetData());
//
//    container::NodeTree::Element<std::string> estr("string");
//    estr.SetData("test");
//    printf("%s\n",estr.GetData().c_str());
//
//    eint5.Insert(eint6);
//    eint5.Insert(eint6);
//    eint5.Insert(eint6);
//    eint1.Insert(eint5);
//    eint1.Insert(eint6);
//    eint4.Insert(eint5);
//    eint4.Insert(eint5);
//    eint4.Insert(eint5);
//
//    estr.Insert(eint);
//    printf("---0---\n");
//    estr.Insert(eint1);
//    printf("---1---\n");
//    estr.Insert(eint2);
//    printf("---2---\n");
//    estr.Insert(eint3);
//    printf("---3---\n");
//    estr.Insert(eint4);
//    printf("---4---\n");
//    auto p = estr.Search<int>("int");
//    if (!p) {
//        printf("not found\n");
//        return 0;
//    }
//    printf("name -- %s  %d \n", p->GetName().c_str(), p->GetData());
//
//    estr.PrintBranch();
//    printf("-------------------------------------------\n");
//    auto p2 = estr.Search<int>("int1/");
//    if (!p2) {
//        printf("not found\n");
//        return 0;
//    }
//    p2->PrintBranch();
//    auto p3 = p2->Clone();
//    p3->PrintBranch();
//    p2->Erase();
//    estr.PrintBranch();
//    printf("-------------------------------------------\n");
//    std::vector<container::NodeTree::Element<int>*> vec;
//    estr.Search<int>("int4/int5/int6", vec);
//    printf("vec num %d\n", vec.size());
//    printf("-------------------------------------------\n");
    container::NodeTree tree("test");
    tree.GetRoot()->Insert<int>("path1",4)->Insert<int>("int", 5)->Insert("path")->Insert<std::string>("name","name");
    tree.GetRoot()->Search<int>("path1")->Insert<char>("char", 5);
    tree.Print();
    container::NodeTree tree1("test1");
    printf("------\n");
    tree1.GetRoot()->Insert<int>("path1231",4)->Insert<int>("i123nt", 5)->Insert("p23ath")->Insert<std::string>("name","name");
    tree1.GetRoot()->Search<int>("path1231")->Insert<char>("czzhar", 5);
    tree1.Print();

    tree = tree1;
    tree.Print();

    tree.GetRoot()->Erase();

    return 0;
}
