#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "time_log.h"

#define TEST_LOG "test"

namespace infra::time {

class DeviceInterfaceMock : public log::DeviceInterface {
public:
    std::string logstr_;
    MOCK_METHOD3(write, size_t(std::string& appname __attribute__ ((unused)), log::Priority&& priotity __attribute__ ((unused)), const std::string& logstr));
};

class LogTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        log::Device::Instance().SetInterface<DeviceInterfaceMock>();
    }
    virtual void TearDown() {

    }
};

TEST_F(LogTest, LogEmergency) {
    std::string logstr = "";
    DeviceInterfaceMock& deviceInterfaceMock = static_cast<DeviceInterfaceMock&>(*(log::Device::Instance().GetInterface()));
    EXPECT_CALL(deviceInterfaceMock, write(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::SaveArg<2>(&logstr), ::testing::Return(0)));
    Log::Emergency(TEST_LOG);
    EXPECT_GT(logstr.size(), 0ul);
}

TEST_F(LogTest, LogAlert) {
    std::string logstr = "";
    DeviceInterfaceMock& deviceInterfaceMock = static_cast<DeviceInterfaceMock&>(*(log::Device::Instance().GetInterface()));
    EXPECT_CALL(deviceInterfaceMock, write(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::SaveArg<2>(&logstr), ::testing::Return(0)));
    Log::Alert(TEST_LOG);
    EXPECT_GT(logstr.size(), 0ul);
}

TEST_F(LogTest, LogCritical) {
    std::string logstr = "";
    DeviceInterfaceMock& deviceInterfaceMock = static_cast<DeviceInterfaceMock&>(*(log::Device::Instance().GetInterface()));
    EXPECT_CALL(deviceInterfaceMock, write(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::SaveArg<2>(&logstr), ::testing::Return(0)));
    Log::Critical(TEST_LOG);
    EXPECT_GT(logstr.size(), 0ul);
}

TEST_F(LogTest, LogError) {
    std::string logstr = "";
    DeviceInterfaceMock& deviceInterfaceMock = static_cast<DeviceInterfaceMock&>(*(log::Device::Instance().GetInterface()));
    EXPECT_CALL(deviceInterfaceMock, write(::testing::_, ::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::SaveArg<2>(&logstr), ::testing::Return(0)));
    Log::Error(TEST_LOG);
    EXPECT_GT(logstr.size(), 0ul);
}

}
