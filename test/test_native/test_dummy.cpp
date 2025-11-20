#include "dummy_test.h"
#include <gtest/gtest.h>
TEST(MathUtilsTest, HandlesPositiveNumbers) {
  // EXPECT_EQ continues running the test even if it fails
  EXPECT_EQ(add(2, 2), 4);
  EXPECT_EQ(add(10, 5), 15);
}

TEST(MathUtilsTest, HandlesNegativeNumbers) {
  EXPECT_EQ(add(-2, -2), -4);
  EXPECT_EQ(add(-5, 2), -3);
}

TEST(ParityTest, DetectsEvens) {
  // ASSERT_TRUE stops the current test immediately if it fails
  ASSERT_TRUE(isEven(4));
  ASSERT_FALSE(isEven(5));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
