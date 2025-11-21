#include <gtest/gtest.h>
#include "dummy_test.h"
TEST(MathUtilsTest, HandlesPositiveNumbers) {
  EXPECT_EQ(add(2, 2), 4);
  EXPECT_EQ(add(10, 5), 15);
}

TEST(MathUtilsTest, HandlesNegativeNumbers) {
  EXPECT_EQ(add(-2, -2), -4);
  EXPECT_EQ(add(-5, 2), -3);
}

TEST(ParityTest, DetectsEvens) {
  ASSERT_TRUE(isEven(4));
  ASSERT_FALSE(isEven(5));
}
