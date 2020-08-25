#include "object.h"
#include <stdio.h>

class ChildObject : public infra::base::Object {
public:
    ChildObject() : infra::base::Object() {
        p = new int;
    }
    ~ChildObject() {
        delete p;
    }

    void test() {
        int *a = new int;
        delete a;
    }

    int* p;
};

int main()
{
    ChildObject o;
    o.test();
    auto c = new ChildObject;
    return 0;
}
