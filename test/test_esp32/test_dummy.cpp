#include <Arduino.h>
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

void setup() {
  // should be the same value as for the `test_speed` option in "platformio.ini"
  // default value is test_speed=115200
  Serial.begin(115200);

  ::testing::InitGoogleTest();
  // if you plan to use GMock, replace the line above with
  // ::testing::InitGoogleMock();
}

void loop() {
  // Run tests
  if (RUN_ALL_TESTS())
    ;

  // sleep for 1 sec
  delay(1000);
}
