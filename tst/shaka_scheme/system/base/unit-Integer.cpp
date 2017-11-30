//
// Created by BeejayInes on 11/15/2017.
//

#include <gmock/gmock.h>
#include <shaka_scheme/system/base/Number/Integer.hpp>

TEST(IntegerUnitTest, add_two_integer_numbers) {
  shaka::Integer r1(3);
  shaka::Integer r2(4);

  shaka::Integer r3(r1 + r2);

  ASSERT_EQ(r3.get_value(), static_cast<int>(7));
}

TEST(IntegerUnitTest, sub_two_integer_numbers) {
  shaka::Integer r1(2);
  shaka::Integer r2(1);

  shaka::Integer r3(r1 - r2);

  ASSERT_EQ(r3.get_value(), static_cast<int>(1));
}

TEST(IntegerUnitTest, mul_two_integer_numbers) {
  shaka::Integer r1(3);
  shaka::Integer r2(3);

  shaka::Integer r3(r1 * r2);

  ASSERT_EQ(r3.get_value(), static_cast<int>(9));
}

TEST(IntegerUnitTest, div_two_integer_numbers) {
  shaka::Integer r1(8);
  shaka::Integer r2(4);

  shaka::Rational r3(r1 / r2);

  ASSERT_EQ(r3.get_numerator(), static_cast<float>(2));
  ASSERT_EQ(r3.get_denominator(), static_cast<float>(1));
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();

}

