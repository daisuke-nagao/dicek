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

TEST(vectorIteratorTest, legacy_iterator) {
  using fvector         = vector<float>;
  using iterator        = fvector::iterator;
  using iterator_traits = std::iterator_traits<iterator>;

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

TEST(vectorIteratorTest, legacy_const_iterator) {
  using fvector         = vector<float>;
  using iterator        = fvector::const_iterator;
  using iterator_traits = std::iterator_traits<iterator>;

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

TEST(vectorIteratorTest, legacy_input_iterator) {
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

TEST(vectorIteratorTest, legacy_const_input_iterator) {
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

#if 0
TEST(vectorIteratorTest, output_iterator) {
  EXPECT_TRUE(false);
}
#endif

TEST(vectorIteratorTest, legacy_forward_iterator) {
  using fvector         = vector<float>;
  using iterator        = fvector::iterator;
  using iterator_traits = std::iterator_traits<iterator>;

  iterator i;

  static_assert(std::is_same<iterator_traits::reference, float&>::value, "LegacyForwardIterator");
  static_assert(std::is_same<iterator, decltype(i++)>::value, "LegacyForwardIterator");
  static_assert(std::is_same<iterator_traits::reference, decltype(*i++)>::value, "LegacyForwardIterator");

  fvector v(5);
  iterator a = std::next(std::begin(v), 3);
  iterator b = std::begin(v) + 3;
  EXPECT_EQ(a, b);
  EXPECT_EQ(&*a, &*b);

  float x;
  a = iterator(&x);
  b = iterator(&x);
  EXPECT_EQ(a, b);

  a = std::next(std::begin(v), 2);
  (void)++iterator(a);
  EXPECT_EQ(2, std::distance(std::begin(v), a));

  a = std::next(std::begin(v), 3);
  b = std::begin(v) + 3;
  EXPECT_TRUE((a == b));
  EXPECT_TRUE((++a == ++b));
}

#if 0
TEST(vectorIteratorTest, bidirectional_iterator) {
  EXPECT_TRUE(false);
}
#endif

#if 0
TEST(vectorIteratorTest, random_access_iterator) {
  EXPECT_TRUE(false);
}
#endif

#ifdef __cpp_concepts
namespace {
template<typename _Tp>
using with_ref = _Tp&;

template<typename _Tp>
concept can_reference = requires {
  typename with_ref<_Tp>;
};

template<class I>
concept LegacyIterator = requires(I i) {
  { *i } -> can_reference;
  { ++i } -> std::same_as<I&>;
  { *i++ } -> can_reference;
}
&&std::copyable<I>;

template<class I>
concept LegacyInputIterator = LegacyIterator<I> && std::equality_comparable<I> && requires(I i) {
  typename std::incrementable_traits<I>::difference_type;
  typename std::indirectly_readable_traits<I>::value_type;
  typename std::common_reference_t<std::iter_reference_t<I>&&, typename std::indirectly_readable_traits<I>::value_type&>;
  *i++;
  typename std::common_reference_t<decltype(*i++)&&, typename std::indirectly_readable_traits<I>::value_type&>;
  requires std::signed_integral<typename std::incrementable_traits<I>::difference_type>;
};

template<class I>
concept LegacyForwardIterator = LegacyInputIterator<I> && std::constructible_from<I> && std::is_lvalue_reference_v<std::iter_reference_t<I>> && std::same_as < std::remove_cvref_t<std::iter_reference_t<I>>,
typename std::indirectly_readable_traits<I>::value_type > &&requires(I i) {
  { i++ } -> std::convertible_to<const I&>;
  { *i++ } -> std::same_as<std::iter_reference_t<I>>;
};

template<class I>
concept LegacyBidirectionalIterator = LegacyForwardIterator<I> && requires(I i) {
  { --i } -> std::same_as<I&>;
  { i-- } -> std::convertible_to<const I&>;
  { *i-- } -> std::same_as<std::iter_reference_t<I>>;
};

template<class I>
concept LegacyRandomAccessIterator = LegacyBidirectionalIterator<I> && std::totally_ordered<I> && requires(I i, typename std::incrementable_traits<I>::difference_type n) {
  { i += n } -> std::same_as<I&>;
  { i -= n } -> std::same_as<I&>;
  { i + n } -> std::same_as<I>;
  { n + i } -> std::same_as<I>;
  { i - n } -> std::same_as<I>;
  { i - i } -> std::same_as<decltype(n)>;
  { i[n] } -> std::convertible_to<std::iter_reference_t<I>>;
};

template<typename T>
requires std::random_access_iterator<T> && std::output_iterator<T, typename std::iterator_traits<T>::value_type> && LegacyRandomAccessIterator<T>
class Checker {
};

template<typename T>
requires std::random_access_iterator<T> && LegacyRandomAccessIterator<T>
class ConstChecker {
};
}  // namespace

TEST(vectorIteratorTest, cxx20_iterator_concepts) {
  using iterator1 = vector<float>::iterator;
  using Check1    = Checker<iterator1>;
  iterator1 i1    = iterator1();
  3 + i1;

  using iterator2 = vector<std::complex<double>>::iterator;
  using Check2    = Checker<iterator2>;
  iterator2 i2    = iterator2();
  5 + i2;

  using const_iterator1 = vector<float>::const_iterator;
  using ConstCheck1     = ConstChecker<const_iterator1>;
  const_iterator1 ci1   = const_iterator1();
  3 + ci1;

  using const_iterator2 = vector<std::complex<double>>::const_iterator;
  using ConstCheck2     = ConstChecker<const_iterator2>;
  const_iterator2 ci2   = const_iterator2();
  5 + ci2;
}
#endif /* __cpp_concepts */
