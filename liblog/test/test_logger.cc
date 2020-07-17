#include <iostream>
#include <unistd.h>
#include "log_interface.h"

int main()
{
    infra::LogFormat f;
    f << infra::LogFormat::Field::Tid << infra::LogFormat::Field::Function << infra::LogFormat::Field::TimeStamp;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    infra::LogFormat::Field a;
    f >> infra::LogFormat::Field();
    f >> a;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    std::cout << (int)a << std::endl;

    infra::LogInterface app;
    app.Debug("testtestasdqwqqwdqwd",1,123,1.1,"asd");
    return 0;
}
