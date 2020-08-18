#include "gmock/gmock.h"
#include "gtest/gtest.h"


int main(int argc, char* argv[]) {
    int rc = 0;
    ::testing::InitGoogleMock(&argc, argv);
    rc = RUN_ALL_TESTS();
    return rc;
}
