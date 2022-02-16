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
  EXPECT_FALSE(vec.ref_count());
}

TEST(vectorTest, copy_constructor) {
  using type = scalar_traits<float>;
  vector<type> vec(3);
  auto copy_vec = vec;

  EXPECT_EQ(vec.data(), copy_vec.data());
  EXPECT_EQ(2, copy_vec.ref_count());
}

TEST(vectorTest, move_constructor) {
  using type = scalar_traits<float>;
  vector<type> vec(5);
  auto data     = vec.data();
  auto move_vec = std::move(vec);

  EXPECT_EQ(data, move_vec.data());
  EXPECT_TRUE(move_vec.ref_count());
  EXPECT_EQ(1, move_vec.ref_count());
}

TEST(vectorTest, clone) {
  using namespace std::literals::complex_literals;

  std::pmr::synchronized_pool_resource sync_mr;

  using type = scalar_traits<std::complex<float>>;
  vector<type> vec(5, &sync_mr);
  std::array<std::complex<float>, 5> check = {
      1.0f + 2.0fi, 3.0f + 4.0fi, 5.0f + 6.0fi, 7.0f + 8.0fi, 9.0f + 10.0fi,
  };

  for (std::size_t i = 0; i < vec.size(); ++i) {
    vec.at(i) = check.at(i);
  }

  vector<type> vec_clone = vec.clone();
  EXPECT_EQ(vec.size(), vec_clone.size());
  EXPECT_EQ(vec.get_allocator(), vec_clone.get_allocator());

  for (std::size_t i = 0; i < vec_clone.size(); ++i) {
    EXPECT_EQ(check.at(i), vec_clone.at(i));
  }

  std::pmr::unsynchronized_pool_resource unsync_mr;
  vector<type> vec_clone_alloc = vec.clone(&unsync_mr);
  EXPECT_EQ(vec.size(), vec_clone_alloc.size());

  for (std::size_t i = 0; i < vec_clone_alloc.size(); ++i) {
    EXPECT_EQ(check.at(i), vec_clone_alloc.at(i));
  }
}

TEST(vectorTest, get_allocator) {
  using namespace std::literals::complex_literals;

  std::pmr::unsynchronized_pool_resource mr;
  using type = scalar_traits<std::complex<float>>;
  vector<type> vec(5, &mr);

  EXPECT_TRUE(std::pmr::polymorphic_allocator<std::byte>(&mr) == vec.get_allocator());
}

TEST(vectorTest, iterator) {
  using namespace std::literals::complex_literals;

  std::pmr::unsynchronized_pool_resource mr;
  using type = scalar_traits<std::complex<float>>;
  vector<type> vec(5, &mr);

  for (std::size_t i = 0; i < vec.size(); ++i) {
    vec.at(i) = i + 1;
  }

  vector<type>::iterator bite = vec.begin();
  EXPECT_EQ(vec.at(0), *bite);
  vector<type>::iterator eite = vec.end();

  auto d = std::distance(bite, eite);
  EXPECT_EQ(vec.size(), d);

  const auto& cvec = vec;

  auto cbite = cvec.begin();
  auto ceite = cvec.end();

  EXPECT_EQ(bite, cbite);
  EXPECT_EQ(eite, ceite);

  EXPECT_EQ(cbite, cvec.cbegin());
  EXPECT_EQ(ceite, cvec.cend());
}
