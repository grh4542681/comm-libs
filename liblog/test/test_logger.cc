#include <iostream>
#include <unistd.h>
#include "log_interface.h"

int main()
{
    infra::log::Format f;
    f << infra::log::Format::Field::Tid << infra::log::Format::Field::Function << infra::log::Format::Field::TimeStamp;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    infra::log::Format::Field a;
    f >> infra::log::Format::Field();
    f >> a;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    std::cout << (int)a << std::endl;

    infra::log::Interface app;
    app.Debug("testtestasdqwqqwdqwd",1,123,1.1,"asd");
    return 0;
}
