#include <iostream>
#include <unistd.h>
#include "log_interface.h"

int main()
{
    log::LogFormat f;
    f << log::LogFormat::Field::Tid << log::LogFormat::Field::Function << log::LogFormat::Field::TimeStamp;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    log::LogFormat::Field a;
    f >> log::LogFormat::Field();
    f >> a;
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    for (auto it : f) {
        std::cout << (int)it << std::endl;
    }
    std::cout << "size - " << f.field_vec_.size() << std::endl;
    std::cout << (int)a << std::endl;

    log::LogInterface app;
    app.Debug("testtestasdqwqqwdqwd",1,123,1.1,"asd");
    return 0;
}
