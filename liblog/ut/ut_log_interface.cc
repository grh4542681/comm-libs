#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "log_interface.h"

#define TEST_LOG "test"

namespace infra::log {

class DeviceInterfaceMock : public DeviceInterface {
public:
    std::string logstr_;
    MOCK_METHOD3(write, size_t(std::string& appname __attribute__ ((unused)), Priority&& priotity __attribute__ ((unused)), const std::string& logstr));
};

class InterfaceTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        Device::Instance().SetInterface<DeviceInterfaceMock>();
    }
    virtual void TearDown() {

    }
    Interface log;
};

TEST_F(InterfaceTest, Emergency) {
    std::string logstr = "";
    DeviceInterfaceMock& deviceInterfaceMock = static_cast<DeviceInterfaceMock&>(*(Device::Instance().GetInterface()));
    EXPECT_CALL(deviceInterfaceMock, write(::testing::_, ::testing::_, ::testing::_))
        //.WillOnce(::testing::SaveArg<2>(&logstr));
        .WillOnce(::testing::DoAll(::testing::SaveArg<2>(&logstr), ::testing::Return(0)));
    log.Emergency(TEST_LOG);
    std::cout<<"--grh--"<<logstr<<std::endl;
}

}
