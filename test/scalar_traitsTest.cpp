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

#include "dicek/scalar_traits.hpp"

TEST(scalar_traitsTest, plain_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<float>::type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<float>::type>::value));

  static_assert(std::is_same<double, typename dicek::math::scalar_traits<double>::type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<double, typename dicek::math::scalar_traits<double>::type>::value));
}

TEST(scalar_traitsTest, const_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<const float>::type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<const float>::type>::value));
}

TEST(scalar_traitsTest, volatile_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<volatile float>::type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<volatile float>::type>::value));
}

TEST(scalar_traitsTest, reference_type) {
  static_assert(std::is_same<float, typename dicek::math::scalar_traits<float&>::type>::value, "two types are not the same");
  EXPECT_TRUE((std::is_same<float, typename dicek::math::scalar_traits<float&>::type>::value));
}
