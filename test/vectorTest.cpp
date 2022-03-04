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

#include <array>
#include <dicek/linalg/vector.hpp>
#include <memory_resource>
#include <utility>

template<typename scalar_traits>
using vector = dicek::math::linalg::vector<scalar_traits>;

template<typename scalar_type>
using scalar_traits = dicek::math::scalar_traits<scalar_type>;

TEST(vectorTest, default_constructor) {
  using type = double;
  vector<type> empty;
  const auto& cempty = const_cast<const decltype(empty)&>(empty);

  EXPECT_EQ(0, empty.size());
  EXPECT_EQ(0, cempty.size());

  static_assert(std::is_same<decltype(empty)::scalar_type, double>::value, "scalar type is not double");
}

TEST(vectorTest, size_constructor) {
  using type = float;
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

  for (std::size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(&cvec.at(i), &cvec[i]);
    EXPECT_EQ(&vec.at(i), &vec[i]);
  }
}

TEST(vectorTest, size_and_allocator_constructor) {
  using type = float;
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
  using type                     = float;
  std::pmr::memory_resource* pmr = std::pmr::null_memory_resource();
  EXPECT_THROW(vector<type>(3, pmr), std::bad_alloc);
}

TEST(vectorTest, external_buffer_constructor) {
  std::array<double, 5> buf = {1, 2, 3, 4, 5};
  const auto check          = buf;
  constexpr auto N          = buf.size();

  using type = double;
  vector<type> vec(buf.data(), buf.size());

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(check.at(i), cvec.at(i));
  }

  EXPECT_EQ(buf.data(), cvec.data());
  EXPECT_EQ(buf.data(), vec.data());
  EXPECT_FALSE(vec.ref_count());
}

TEST(vectorTest, external_buffer_constructor_with_step) {
  std::array<double, 5> buf = {1, 2, 3, 4, 5};
  const auto check          = buf;
  constexpr auto N          = 3;

  using type = double;
  vector<type> vec(buf.data(), buf.size(), 2);

  EXPECT_EQ(N, vec.size());
  for (std::size_t i = 0; i < N; ++i) {
    EXPECT_FLOAT_EQ(buf.at(2 * i), vec.at(i));
  }
}

TEST(vectorTest, copy_constructor) {
  using type = float;
  vector<type> vec(3);
  auto copy_vec = vec;

  EXPECT_EQ(vec.data(), copy_vec.data());
  EXPECT_EQ(2, copy_vec.ref_count());
}

TEST(vectorTest, move_constructor) {
  using type = float;
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

  using type = std::complex<float>;
  vector<type> vec(5, &sync_mr);
  std::array<std::complex<float>, 5> check = {
      1.0f + 2.0if, 3.0f + 4.0if, 5.0f + 6.0if, 7.0f + 8.0if, 9.0f + 10.0if,
  };

  for (std::size_t i = 0; i < vec.size(); ++i) {
    vec.at(i) = check.at(i);
  }

  vector<type> vec_clone = const_cast<const vector<type>&>(vec).clone();
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
  using type = std::complex<float>;
  vector<type> vec(5, &mr);

  EXPECT_TRUE(vec.get_allocator()->is_equal(mr));
}

TEST(vectorTest, begin_end) {
  using namespace std::literals::complex_literals;

  std::pmr::unsynchronized_pool_resource mr;
  using type = std::complex<float>;
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

  using const_iterator = vector<type>::const_iterator;
  EXPECT_EQ(const_iterator(bite), cbite);
  EXPECT_EQ(const_iterator(eite), ceite);

  EXPECT_EQ(cbite, cvec.cbegin());
  EXPECT_EQ(ceite, cvec.cend());
}

TEST(vectorTest, swap) {
  using std::swap;
  using type = float;
  vector<float> v1(10), v2(5);
  float x[10] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  };
  float y[5] = {
      10, 20, 30, 40, 50,
  };

  auto ptr1 = v1.data();
  auto ptr2 = v2.data();

  for (std::size_t i = 0; i < v1.size(); ++i) {
    v1.at(i) = x[i];
  }
  for (std::size_t i = 0; i < v2.size(); ++i) {
    v2.at(i) = y[i];
  }

  swap(v1, v2);
  EXPECT_EQ(std::size(x), v2.size());
  EXPECT_EQ(std::size(y), v1.size());

  for (std::size_t i = 0; i < v1.size(); ++i) {
    EXPECT_FLOAT_EQ(y[i], v1.at(i));
  }
  for (std::size_t i = 0; i < v2.size(); ++i) {
    EXPECT_FLOAT_EQ(x[i], v2.at(i));
  }

  EXPECT_EQ(ptr1, v2.data());
  EXPECT_EQ(ptr2, v1.data());

  v1.swap(v2);
  EXPECT_EQ(std::size(x), v1.size());
  EXPECT_EQ(std::size(y), v2.size());

  for (std::size_t i = 0; i < v1.size(); ++i) {
    EXPECT_FLOAT_EQ(x[i], v1.at(i));
  }
  for (std::size_t i = 0; i < v2.size(); ++i) {
    EXPECT_FLOAT_EQ(y[i], v2.at(i));
  }

  EXPECT_EQ(ptr1, v1.data());
  EXPECT_EQ(ptr2, v2.data());
}

TEST(vectorTest, move_assignment) {
  using type = double;
  vector<type> v1(10);
  float x[10] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  };
  for (std::size_t i = 0; i < v1.size(); ++i) {
    v1.at(i) = x[i];
  }
  v1 = std::move(v1);
  EXPECT_TRUE(v1.data() != nullptr);
}

TEST(vectorTest, copy_assignment) {
  using type = float;
  vector<type> v1(10), v2(5);
  float x[10] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  };
  float y[5] = {
      10, 20, 30, 40, 50,
  };

  for (std::size_t i = 0; i < v1.size(); ++i) {
    v1.at(i) = x[i];
  }
  for (std::size_t i = 0; i < v2.size(); ++i) {
    v2.at(i) = y[i];
  }

  v2 = v1;

  EXPECT_EQ(2, v1.ref_count());
  EXPECT_EQ(2, v2.ref_count());
  EXPECT_EQ(v1.data(), v2.data());
  EXPECT_EQ(v1.size(), v2.size());
}

TEST(vectorTest, initializer_list) {
  using type = float;
  vector<type> v1({1.0, 2.0, 3.0});

  EXPECT_EQ(3, v1.size());
  for (std::size_t i = 0; i < v1.size(); ++i) {
    EXPECT_FLOAT_EQ(static_cast<type>(i + 1), v1.at(i));
  }

  std::pmr::unsynchronized_pool_resource mr;
  vector<type> v2({10, 20, 30, 40}, &mr);
  EXPECT_EQ(4, v2.size());
  for (std::size_t i = 0; i < v2.size(); ++i) {
    EXPECT_FLOAT_EQ(static_cast<type>(i + 1) * 10, v2.at(i));
  }
  EXPECT_TRUE(v2.get_allocator()->is_equal(mr));
}

TEST(vectorTest, iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::iterator;
  static_assert(std::is_same<std::iterator_traits<iterator>::value_type, float>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::difference_type, ptrdiff_t>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::pointer, float*>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::reference, float&>::value, "LegacyIterator");
  //static_assert(std::is_same<std::iterator_traits<iterator>::iterator_category, std::random_access_iterator_tag>::value, "LegacyIterator");

  iterator default_constructible;
  iterator copy_constructible = default_constructible;
  iterator copy_assignable;
  copy_assignable = default_constructible;

  static_assert(std::is_same<iterator::reference, decltype(*default_constructible)>::value, "LegacyIterator");
  static_assert(std::is_same<iterator&, decltype(++default_constructible)>::value, "LegacyIterator");

  fvector v(5);
  auto b = std::begin(v);
  auto e = std::end(v);

  using std::swap;
  swap(b, e);
  swap(e, b);
}

TEST(vectorTest, const_iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::const_iterator;
  static_assert(std::is_same<std::iterator_traits<iterator>::value_type, const float>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::difference_type, ptrdiff_t>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::pointer, const float*>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::reference, const float&>::value, "LegacyIterator");
  //static_assert(std::is_same<std::iterator_traits<iterator>::iterator_category, std::random_access_iterator_tag>::value, "LegacyIterator");

  iterator default_constructible;
  iterator copy_constructible = default_constructible;
  iterator copy_assignable;
  copy_assignable = default_constructible;

  static_assert(std::is_same<iterator::reference, decltype(*default_constructible)>::value, "LegacyIterator");
  static_assert(std::is_same<iterator&, decltype(++default_constructible)>::value, "LegacyIterator");

  fvector v(5);
  auto b = std::cbegin(v);
  auto e = std::cend(v);

  using std::swap;
  swap(b, e);
  swap(e, b);
}

TEST(vectorTest, input_iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::iterator;

  fvector v(5);
  for (std::size_t i = 0; i < v.size(); ++i) {
    v.at(i) = (i + 1) * M_PI;
  }
  auto b1 = std::begin(v);
  auto b2 = std::begin(v);
  auto e1 = std::end(v);
  auto e2 = e1;
  EXPECT_FALSE(b1 == e1);
  EXPECT_TRUE(b1 == b2);
  EXPECT_TRUE(e1 == e2);

  EXPECT_EQ(!(b1 == e1), (b1 != e1));
  EXPECT_EQ(!(b1 == b2), (b1 != b2));
  EXPECT_EQ(!(e1 == e2), (e1 != e2));

  static_assert(std::is_same<std::iterator_traits<iterator>::reference, decltype(*b1)>::value, "LegacyInputIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::value_type, std::remove_reference<decltype(*b1)>::type>::value, "LegacyInputIterator");

  EXPECT_EQ(b1.operator->(), &*b1);
}
