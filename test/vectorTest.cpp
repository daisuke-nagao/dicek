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

#include <dicek/linalg/vector.hpp>
#include <dicek/scalar_traits.hpp>
#include <memory_resource>

template<typename scalar_traits>
using vector = dicek::math::linalg::vector<scalar_traits>;

template<typename scalar_type>
using scalar_traits = dicek::math::scalar_traits<scalar_type>;

TEST(vectorTest, default_constructor) {
  using type = scalar_traits<double>;
  vector<type> empty;
  const auto& cempty = const_cast<const decltype(empty)&>(empty);

  EXPECT_EQ(0, empty.size());
  EXPECT_EQ(0, cempty.size());

  static_assert(std::is_same<decltype(empty)::scalar_type, double>::value, "scalar type is not double");
}

TEST(vectorTest, size_constructor) {
  using type = scalar_traits<float>;
  vector<type> vec(3);

  EXPECT_EQ(3, vec.size());

  static_assert(std::is_same<decltype(vec)::scalar_type, float>::value, "scalar type is not float");

  for (size_t i = 0; i < vec.size(); ++i) {
    vec.at(i) = static_cast<decltype(vec)::scalar_type>(i + 1);
  }

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(static_cast<decltype(vec)::scalar_type>(i + 1), cvec.at(i));
  }

  EXPECT_THROW(cvec.at(3), std::out_of_range);
  EXPECT_THROW(vec.at(4), std::out_of_range);
}

TEST(vectorTest, size_and_allocator_constructor) {
  using type = scalar_traits<float>;
  std::pmr::unsynchronized_pool_resource mr;
  vector<type> vec(3, &mr);

  static_assert(std::is_same<decltype(vec)::scalar_type, float>::value, "scalar type is not float");

  for (size_t i = 0; i < vec.size(); ++i) {
    vec.at(i) = static_cast<decltype(vec)::scalar_type>(i + 1);
  }

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(static_cast<decltype(vec)::scalar_type>(i + 1), cvec.at(i));
  }
}

TEST(vectorTest, size_and_null_allocator_constructor) {
  using type                     = scalar_traits<float>;
  std::pmr::memory_resource* pmr = std::pmr::null_memory_resource();
  EXPECT_THROW(vector<type>(3, pmr), std::bad_alloc);
}

TEST(vectorTest, external_buffer_constructor) {
  std::array<double, 5> buf = {1, 2, 3, 4, 5};
  const auto check          = buf;
  constexpr auto N          = buf.size();

  using type = scalar_traits<double>;
  vector<type> vec(buf.data(), buf.size());

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(check.at(i), cvec.at(i));
  }

  EXPECT_EQ(buf.data(), cvec.data());
  EXPECT_EQ(buf.data(), vec.data());
}

TEST(vectorTest, copy_constructor) {
  using type = scalar_traits<float>;
  vector<type> vec(3);
  auto copy_vec = vec;
}
