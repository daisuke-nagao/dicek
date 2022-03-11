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

TEST(vectorIteratorTest, iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::iterator;
  static_assert(std::is_same<std::iterator_traits<iterator>::value_type, float>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::difference_type, ptrdiff_t>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::pointer, float*>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::reference, float&>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::iterator_category, std::random_access_iterator_tag>::value, "LegacyIterator");

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

TEST(vectorIteratorTest, const_iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::const_iterator;
  static_assert(std::is_same<std::iterator_traits<iterator>::value_type, const float>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::difference_type, ptrdiff_t>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::pointer, const float*>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::reference, const float&>::value, "LegacyIterator");
  static_assert(std::is_same<std::iterator_traits<iterator>::iterator_category, std::random_access_iterator_tag>::value, "LegacyIterator");

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

TEST(vectorIteratorTest, input_iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::iterator;

  fvector v(5);
  for (std::size_t i = 0; i < v.size(); ++i) {
    v.at(i) = (i + 1) * 3.14;
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
  (void)b1++;

  auto tmp1 = std::begin(v);
  auto tmp2 = tmp1;

  auto foo = *tmp1++;
  auto x   = *tmp2;
  auto bar = ++tmp2;
  EXPECT_EQ(x, foo);
  EXPECT_EQ(tmp1, tmp2);
}

TEST(vectorIteratorTest, const_input_iterator) {
  using fvector  = vector<float>;
  using iterator = fvector::const_iterator;

  fvector v(5);
  for (std::size_t i = 0; i < v.size(); ++i) {
    v.at(i) = (i + 1) * 3.14;
  }
  auto b1 = std::cbegin(v);
  auto b2 = std::cbegin(v);
  auto e1 = std::cend(v);
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
  (void)b1++;

  auto tmp1 = std::cbegin(v);
  auto tmp2 = tmp1;

  auto foo = *tmp1++;
  auto x   = *tmp2;
  auto bar = ++tmp2;
  EXPECT_EQ(x, foo);
  EXPECT_EQ(tmp1, tmp2);
}

#ifdef __cpp_concepts
namespace {
template<typename T>
requires std::random_access_iterator<T> && std::output_iterator<T, typename std::iterator_traits<T>::value_type>
class Checker {
};
template<typename T>
requires std::random_access_iterator<T>
class ConstChecker {
};
}  // namespace
TEST(vectorIteratorTest, cxx20_iterator_concepts) {
  using iterator1 = vector<float>::iterator;
  using Check1    = Checker<iterator1>;
  iterator1 i1    = iterator1();
  iterator1::difference_type(3) + i1;

  using iterator2 = vector<std::complex<double>>::iterator;
  using Check2    = Checker<iterator2>;
  iterator2 i2    = iterator2();
  iterator2::difference_type(5) + i2;

  using const_iterator1 = vector<float>::const_iterator;
  using ConstCheck1     = ConstChecker<const_iterator1>;
  const_iterator1 ci1   = const_iterator1();
  const_iterator1::difference_type(3) + ci1;

  using const_iterator2 = vector<std::complex<double>>::const_iterator;
  using ConstCheck2     = ConstChecker<const_iterator2>;
  const_iterator2 ci2   = const_iterator2();
  const_iterator2::difference_type(5) + ci2;
}
#endif /* __cpp_concepts */
