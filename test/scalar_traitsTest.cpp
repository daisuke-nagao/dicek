/*
MIT License

Copyright (c) 2022 Daisuke NAGAO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <gtest/gtest.h>

#include <type_traits>

#include <dicek/scalar_traits.hpp>

TEST(scalar_traitsTest, plain_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<float>::scalar_type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<float>::scalar_type>::value));

  static_assert(std::is_same<double, typename dicek::math::scalar_traits<double>::scalar_type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<double, typename dicek::math::scalar_traits<double>::scalar_type>::value));
}

TEST(scalar_traitsTest, const_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<const float>::scalar_type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<const float>::scalar_type>::value));
}

TEST(scalar_traitsTest, volatile_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<volatile float>::scalar_type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<volatile float>::scalar_type>::value));
}

TEST(scalar_traitsTest, reference_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<float&>::scalar_type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<float&>::scalar_type>::value));
}

TEST(scalar_traitsTest, conj) {
  EXPECT_EQ(dicek::math::scalar_traits<float>::conj(1.5), 1.5f);
  EXPECT_EQ(dicek::math::scalar_traits<std::complex<double>>::conj(std::complex<double>(1.5, -2)), std::complex<double>(1.5, 2));
}

TEST(scalar_traitsTest, abs) {
  EXPECT_EQ(dicek::math::scalar_traits<float>::abs(-1.5f), 1.5f);
  EXPECT_EQ(dicek::math::scalar_traits<std::complex<double>>::abs(std::complex<double>(-1.5f, 2.0f)), sqrt(1.5 * 1.5 + 4));
}
