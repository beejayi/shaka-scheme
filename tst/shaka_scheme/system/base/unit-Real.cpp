//
// Created by BeejayInes on 11/15/2017.
//

#include <gmock/gmock.h>
#include <shaka_scheme/system/base/Number/Real.hpp>

TEST(RealUnitTest, add_two_real_numbers) {
  shaka::Real r1(3);
  shaka::Real r2(4);

  shaka::Real r3(r1 + r2);

  ASSERT_EQ(r3.get_value(), static_cast<float>(7));
}

TEST(RealUnitTest, sub_two_real_numbers) {
  shaka::Real r1(0);
  shaka::Real r2(1);

  shaka::Real r3(r1 - r2);

  ASSERT_EQ(r3.get_value(), static_cast<float>(-1));
}

TEST(RealUnitTest, mul_two_real_numbers) {
  shaka::Real r1(3);
  shaka::Real r2(3);

  shaka::Real r3(r1 * r2);

  ASSERT_EQ(r3.get_value(), static_cast<float>(9));
}

TEST(RealUnitTest, div_two_real_numbers) {
  shaka::Real r1(9);
  shaka::Real r2(4);

  shaka::Real r3(r1 / r2);

  ASSERT_EQ(r3.get_value(), static_cast<float>(2.25));
}
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();

}

