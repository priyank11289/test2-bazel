#include "common/math_utils.h"
#include <gtest/gtest.h>

TEST(MathUtilsTest, CalculateAverage) {
  int data[] = {10, 20, 30};
  EXPECT_DOUBLE_EQ(common::calculateAverage(data, 3), 20.0);
}

TEST(MathUtilsTest, CalculateAverageEmpty) {
  EXPECT_DOUBLE_EQ(common::calculateAverage(nullptr, 0), 0.0);
}
