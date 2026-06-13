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
#include <complex>
#include <dicek/linalg/vector.hpp>
#include <memory_resource>
#include <utility>

template<typename scalar_traits>
using vector = dicek::math::linalg::vector<scalar_traits>;

template<typename scalar_type>
using scalar_traits = dicek::math::scalar_traits<scalar_type>;

class allocation_control_resource : public std::pmr::memory_resource {
 public:
  void reject_allocations() {
    reject_allocations_ = true;
  }

 private:
  void* do_allocate(std::size_t bytes, std::size_t alignment) override {
    if (reject_allocations_) {
      throw std::bad_alloc();
    }
    return std::pmr::new_delete_resource()->allocate(bytes, alignment);
  }

  void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
    std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
  }

  bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
    return this == &other;
  }

  bool reject_allocations_ = false;
};

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
  std::array<double, 9> buf = {1, -1, 2, -1, 3, -1, 4, -1, 5};
  constexpr auto N          = 5;

  using type = double;
  vector<type> vec(buf.data(), N, 2);

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(static_cast<type>(i + 1), cvec.at(i));
  }

  vec.at(1) = 20;
  EXPECT_EQ(20, buf.at(2));
  EXPECT_EQ(-1, buf.at(1));

  std::array<double, N> copied = {};
  std::copy(vec.begin(), vec.end(), copied.begin());
  for (size_t i = 0; i < copied.size(); ++i) {
    EXPECT_EQ(cvec.at(i), copied.at(i));
  }

  EXPECT_EQ(buf.data(), cvec.data());
  EXPECT_EQ(buf.data(), vec.data());
  EXPECT_FALSE(vec.ref_count());
}

TEST(vectorTest, external_buffer_constructor_with_negative_step) {
  std::array<double, 5> buf = {1, 2, 3, 4, 5};
  constexpr auto N          = buf.size();

  using type = double;
  vector<type> vec(buf.data() + buf.size() - 1, N, -1);

  const auto& cvec = vec;
  for (size_t i = 0; i < vec.size(); ++i) {
    EXPECT_EQ(static_cast<type>(N - i), cvec.at(i));
  }

  vec.at(2) = 30;
  EXPECT_EQ(30, buf.at(2));

  auto bite = vec.begin();
  auto eite = vec.end();
  EXPECT_EQ(vec.size(), std::distance(bite, eite));

  EXPECT_EQ(buf.data() + buf.size() - 1, cvec.data());
  EXPECT_FALSE(vec.ref_count());
}

TEST(vectorTest, external_buffer_constructor_rejects_zero_step) {
  std::array<double, 5> buf = {1, 2, 3, 4, 5};

  using type = double;
  EXPECT_THROW(vector<type>(buf.data(), buf.size(), 0), std::invalid_argument);
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

  EXPECT_EQ(bite, cbite);
  EXPECT_EQ(eite, ceite);

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

TEST(vectorTest, add_and_subtract) {
  vector<double> v1({1.0, 2.0, 3.0});
  vector<double> v2({10.0, 20.0, 30.0});

  auto sum = v1 + v2;
  EXPECT_EQ(v1.size(), sum.size());
  EXPECT_DOUBLE_EQ(11.0, sum.at(0));
  EXPECT_DOUBLE_EQ(22.0, sum.at(1));
  EXPECT_DOUBLE_EQ(33.0, sum.at(2));

  auto difference = v2 - v1;
  EXPECT_EQ(v1.size(), difference.size());
  EXPECT_DOUBLE_EQ(9.0, difference.at(0));
  EXPECT_DOUBLE_EQ(18.0, difference.at(1));
  EXPECT_DOUBLE_EQ(27.0, difference.at(2));

  v1 += v2;
  EXPECT_DOUBLE_EQ(11.0, v1.at(0));
  EXPECT_DOUBLE_EQ(22.0, v1.at(1));
  EXPECT_DOUBLE_EQ(33.0, v1.at(2));

  v1 -= v2;
  EXPECT_DOUBLE_EQ(1.0, v1.at(0));
  EXPECT_DOUBLE_EQ(2.0, v1.at(1));
  EXPECT_DOUBLE_EQ(3.0, v1.at(2));
}

TEST(vectorTest, scale_and_divide) {
  vector<double> v({1.0, -2.0, 3.0});

  auto scaled = v.scale(2.5);
  EXPECT_DOUBLE_EQ(2.5, scaled.at(0));
  EXPECT_DOUBLE_EQ(-5.0, scaled.at(1));
  EXPECT_DOUBLE_EQ(7.5, scaled.at(2));

  auto product = 3.0 * v;
  EXPECT_DOUBLE_EQ(3.0, product.at(0));
  EXPECT_DOUBLE_EQ(-6.0, product.at(1));
  EXPECT_DOUBLE_EQ(9.0, product.at(2));

  auto quotient = v / 2.0;
  EXPECT_DOUBLE_EQ(0.5, quotient.at(0));
  EXPECT_DOUBLE_EQ(-1.0, quotient.at(1));
  EXPECT_DOUBLE_EQ(1.5, quotient.at(2));

  v *= -2.0;
  EXPECT_DOUBLE_EQ(-2.0, v.at(0));
  EXPECT_DOUBLE_EQ(4.0, v.at(1));
  EXPECT_DOUBLE_EQ(-6.0, v.at(2));

  v /= 2.0;
  EXPECT_DOUBLE_EQ(-1.0, v.at(0));
  EXPECT_DOUBLE_EQ(2.0, v.at(1));
  EXPECT_DOUBLE_EQ(-3.0, v.at(2));
}

TEST(vectorTest, numerical_operations_accept_strided_external_buffer) {
  std::array<double, 6> lhs_buf = {1.0, -1.0, 2.0, -1.0, 3.0, -1.0};
  std::array<double, 6> rhs_buf = {10.0, -1.0, 20.0, -1.0, 30.0, -1.0};

  vector<double> lhs(lhs_buf.data(), 3, 2);
  vector<double> rhs(rhs_buf.data(), 3, 2);

  auto sum = lhs + rhs;
  EXPECT_DOUBLE_EQ(11.0, sum.at(0));
  EXPECT_DOUBLE_EQ(22.0, sum.at(1));
  EXPECT_DOUBLE_EQ(33.0, sum.at(2));

  lhs += rhs;
  EXPECT_DOUBLE_EQ(11.0, lhs_buf.at(0));
  EXPECT_DOUBLE_EQ(22.0, lhs_buf.at(2));
  EXPECT_DOUBLE_EQ(33.0, lhs_buf.at(4));
  EXPECT_DOUBLE_EQ(-1.0, lhs_buf.at(1));
  EXPECT_DOUBLE_EQ(-1.0, lhs_buf.at(3));
  EXPECT_DOUBLE_EQ(-1.0, lhs_buf.at(5));
}

TEST(vectorTest, in_place_addition_preserves_rhs_values_for_overlapping_views) {
  std::array<double, 4> buf = {1.0, 2.0, 3.0, 4.0};

  vector<double> lhs(buf.data() + 1, 3);
  vector<double> rhs(buf.data(), 3);

  lhs += rhs;

  EXPECT_DOUBLE_EQ(1.0, buf.at(0));
  EXPECT_DOUBLE_EQ(3.0, buf.at(1));
  EXPECT_DOUBLE_EQ(5.0, buf.at(2));
  EXPECT_DOUBLE_EQ(7.0, buf.at(3));
}

TEST(vectorTest, in_place_subtraction_preserves_rhs_values_for_overlapping_views) {
  std::array<double, 4> buf = {1.0, 2.0, 3.0, 4.0};

  vector<double> lhs(buf.data() + 1, 3);
  vector<double> rhs(buf.data(), 3);

  lhs -= rhs;

  EXPECT_DOUBLE_EQ(1.0, buf.at(0));
  EXPECT_DOUBLE_EQ(1.0, buf.at(1));
  EXPECT_DOUBLE_EQ(1.0, buf.at(2));
  EXPECT_DOUBLE_EQ(1.0, buf.at(3));
}

TEST(vectorTest, in_place_operations_do_not_allocate_for_identical_views) {
  allocation_control_resource mr;
  vector<double> v({1.0, 2.0, 3.0}, &mr);
  const auto same_view = v;

  mr.reject_allocations();

  EXPECT_NO_THROW(v += v);
  EXPECT_DOUBLE_EQ(2.0, v.at(0));
  EXPECT_DOUBLE_EQ(4.0, v.at(1));
  EXPECT_DOUBLE_EQ(6.0, v.at(2));

  EXPECT_NO_THROW(v -= same_view);
  EXPECT_DOUBLE_EQ(0.0, v.at(0));
  EXPECT_DOUBLE_EQ(0.0, v.at(1));
  EXPECT_DOUBLE_EQ(0.0, v.at(2));
}

TEST(vectorTest, numerical_operations_reject_size_mismatch) {
  vector<double> v1({1.0, 2.0, 3.0});
  vector<double> v2({10.0, 20.0});

  EXPECT_THROW(v1 + v2, std::invalid_argument);
  EXPECT_THROW(v1 - v2, std::invalid_argument);
  EXPECT_THROW(v1 += v2, std::invalid_argument);
  EXPECT_THROW(v1 -= v2, std::invalid_argument);
  EXPECT_THROW(dot(v1, v2), std::invalid_argument);
  EXPECT_THROW(inner_product(v1, v2), std::invalid_argument);
}

TEST(vectorTest, dot_inner_product_and_norm) {
  vector<double> v1({1.0, 2.0, 3.0});
  vector<double> v2({10.0, 20.0, 30.0});

  EXPECT_DOUBLE_EQ(140.0, dot(v1, v2));
  EXPECT_DOUBLE_EQ(140.0, inner_product(v1, v2));
  EXPECT_DOUBLE_EQ(std::sqrt(14.0), norm(v1, 2.0));
  EXPECT_DOUBLE_EQ(std::pow(36.0, 1.0 / 3.0), norm(v1, 3.0));
  EXPECT_THROW(norm(v1, 0.5), std::range_error);
}

TEST(vectorTest, complex_dot_uses_conjugated_rhs) {
  using namespace std::literals::complex_literals;

  vector<std::complex<double>> v1({1.0 + 4.0i, 2.0 + 5.0i, 3.0 + 6.0i});
  vector<std::complex<double>> v2({2.0 + 3.0i, 5.0 + 7.0i, 11.0 + 13.0i});

  const auto expected = (1.0 + 4.0i) * std::conj(2.0 + 3.0i) + (2.0 + 5.0i) * std::conj(5.0 + 7.0i) + (3.0 + 6.0i) * std::conj(11.0 + 13.0i);
  EXPECT_EQ(expected, dot(v1, v2));
  EXPECT_EQ(expected, inner_product(v1, v2));
  EXPECT_DOUBLE_EQ(std::sqrt(91.0), norm(v1, 2.0));
}
