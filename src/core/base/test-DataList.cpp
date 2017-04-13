#include <gtest/gtest.h>
#include "core/base/DataList.h"

/// @brief Basic default constructor test
TEST(DataList, constructors) {
    shaka::DataList l(nullptr, nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
