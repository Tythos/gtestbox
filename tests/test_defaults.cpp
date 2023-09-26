/**
 * tests/test_defaults.cpp
 */

#include "person.hpp"
#include "gtest/gtest.h"

namespace gtestbox {
    namespace tests {
        namespace test_hello {
            TEST(TestDefaults, BasicAssertions) {
                gtestbox::person::Person p;
                EXPECT_STREQ(p.name.c_str(), "unknown");
                EXPECT_EQ(p.age, 0);
            }
        }
    }
}

int main(int nArgs, char** vArgs) {
    ::testing::InitGoogleTest(&nArgs, vArgs);
    return RUN_ALL_TESTS();
}
