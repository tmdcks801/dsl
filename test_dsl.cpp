#include <gtest/gtest.h>
#include "dsl.h"

TEST(CalculatorTest, HandleAddition) {
    Calculator calc;
    const int a = 2;  
    const int b = 3;
    const int expected = 5;
    EXPECT_EQ(calc.add(a, b), expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}