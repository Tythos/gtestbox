/**
 * 
 */

#include "gtest/gtest.h"

namespace gtestbox {
    namespace tests {
        namespace test_hello {
            TEST(HelloTest, BasicAssertions) {
                EXPECT_STRNE("hello", "world");
                EXPECT_EQ(7 * 6, 42);
            }
        }
    }
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS();
}
