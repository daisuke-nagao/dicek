/*
MIT License

Copyright (c) 2016 Daisuke NAGAO

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
#ifndef UUID_3A12F814_B92D_11E6_AB3C_0800274CD854
#define UUID_3A12F814_B92D_11E6_AB3C_0800274CD854

#include <cxxtest/TestSuite.h>

#include <cfloat>
#include <complex>
#include <typeinfo>

#include "dicek/linalg/vector.hpp"

class vector2cTest : public CxxTest::TestSuite {
 public:
  using complexf = std::complex<float>;
  using vector2c = dicek::math::linalg::vector<3, complexf>;

  void testDIM(void) {
    static_assert(vector2c::DIM == 3, "vector<3,complexf>::DIM is not 3");
    TSM_ASSERT_EQUALS("vector<3,complexf>::DIM is not 3", 3, (vector2c::DIM));
  }

  void test_scalar_type(void) {
    static_assert(std::is_same<vector2c::scalar_type, complexf>::value, "vector2c::scalar_type is not complexf");
    TSM_ASSERT("vector2c::scalar_type is not complexf", typeid(vector2c::scalar_type) == typeid(complexf));
  }

  void test_default_constructor(void) {
    vector2c v;
    TSM_ASSERT_EQUALS("Not zero", v[0], complexf());
    TSM_ASSERT_EQUALS("Not zero", v[1], complexf());
    TSM_ASSERT_EQUALS("Not zero", v[2], complexf());
  }

  void test_at(void) {
    vector2c v;

    v.at(0) = complexf(5, 1);
    v.at(1) = complexf(7, 2);
    v.at(2) = complexf(11, 3);

    TSM_ASSERT_EQUALS("at(0) is not (5, 1)", v.at(0), complexf(5, 1));
    TSM_ASSERT_EQUALS("at(1) is not (7, 2)", v.at(1), complexf(7, 2));
    TSM_ASSERT_EQUALS("at(2) is not (11, 3)", v.at(2), complexf(11, 3));
  }

  void test_at_const(void) {
    vector2c v;

    v.at(0) = complexf(5, 1);
    v.at(1) = complexf(7, 2);
    v.at(2) = complexf(11, 3);

    const auto& ref = v;

    TSM_ASSERT_EQUALS("at(0) is not (5, 1)", v.at(0), complexf(5, 1));
    TSM_ASSERT_EQUALS("at(1) is not (7, 2)", v.at(1), complexf(7, 2));
    TSM_ASSERT_EQUALS("at(2) is not (11, 3)", v.at(2), complexf(11, 3));
  }

  void test_at_throw(void) {
    vector2c v;
    TSM_ASSERT_THROWS("Exception not thrown", v.at(3), std::out_of_range);
    TSM_ASSERT_THROWS("Exception not thrown", const_cast<const decltype(v)&>(v).at(3), std::out_of_range);
  }

  void test_operator_square_bracket(void) {
    vector2c v;
    v[0] = complexf(5, 1);
    v[1] = complexf(7, 2);
    v[2] = complexf(11, 3);

    TSM_ASSERT_EQUALS("operator[](0) is not 5 + i", v[0], complexf(5, 1));
    TSM_ASSERT_EQUALS("operator[](1) is not 7 + 2i", v[1], complexf(7, 2));
    TSM_ASSERT_EQUALS("operator[](2) is not 11 + 3i", v[2], complexf(11, 3));
  }

  void test_operator_square_bracket_const(void) {
    vector2c v;
    v[0] = complexf(5, 1);
    v[1] = complexf(7, 2);
    v[2] = complexf(11, 3);

    const auto& ref = v;

    TSM_ASSERT_EQUALS("operator[](0) is not 5 + i", ref[0], complexf(5, 1));
    TSM_ASSERT_EQUALS("operator[](1) is not 7 + 2i", ref[1], complexf(7, 2));
    TSM_ASSERT_EQUALS("operator[](2) is not 11 + 3i", ref[2], complexf(11, 3));
  }

  void test_operator_add(void) {
    vector2c v, w;
    v[0] = 1;
    v[1] = 2;
    v[2] = 3;

    w[0] = complexf(0, 10);
    w[1] = complexf(0, 20);
    w[2] = complexf(0, 30);

    const auto& _v = v;
    const auto& _w = w;

    vector2c x = _v + _w;
    vector2c y = _w + _v;

    TSM_ASSERT_EQUALS("operator+ is not 11", x[0], complexf(1, 10));
    TSM_ASSERT_EQUALS("operator+ is not 22", x[1], complexf(2, 20));
    TSM_ASSERT_EQUALS("operator+ is not 33", x[2], complexf(3, 30));

    TSM_ASSERT_EQUALS("operator+ is not 11", y[0], x[0]);
    TSM_ASSERT_EQUALS("operator+ is not 22", y[1], x[1]);
    TSM_ASSERT_EQUALS("operator+ is not 33", y[2], x[2]);
  }

  void test_scale(void) {
    vector2c v;
    v[0] = complexf(1, 10);
    v[1] = complexf(2, 20);
    v[2] = complexf(3, 30);

    vector2c w = v.scale(3);
    TS_ASSERT_EQUALS(w[0], complexf(3, 30));
    TS_ASSERT_EQUALS(w[1], complexf(6, 60));
    TS_ASSERT_EQUALS(w[2], complexf(9, 90));
  }

  void test_map(void) {
    vector2c v;
    v[0] = complexf(1, 10);
    v[1] = complexf(2, 20);
    v[2] = complexf(3, 30);

    const auto& _v = v;

    vector2c w = _v.map([](complexf val) { return val * val; });
    TS_ASSERT_EQUALS(w[0], complexf(1, 10) * complexf(1, 10));
    TS_ASSERT_EQUALS(w[1], complexf(2, 20) * complexf(2, 20));
    TS_ASSERT_EQUALS(w[2], complexf(3, 30) * complexf(3, 30));

    vector2c x = _v.map([](complexf val) -> complexf { return val + 2.0f; });
    TS_ASSERT_EQUALS(x[0], complexf(3, 10));
    TS_ASSERT_EQUALS(x[1], complexf(4, 20));
    TS_ASSERT_EQUALS(x[2], complexf(5, 30));
  }

  void test_inner_product(void) {
    vector2c v, w;
    v[0] = complexf(1, 4);
    v[1] = complexf(2, 5);
    v[2] = complexf(3, 6);

    w[0] = complexf(2, 3);
    w[1] = complexf(5, 7);
    w[2] = complexf(11, 13);

    const auto& _v = v;
    const auto& _w = w;

    vector2c::scalar_type x = inner_product(_v, _w);

    TS_ASSERT_EQUALS(x, complexf(1, 4) * std::conj(complexf(2, 3)) + complexf(2, 5) * std::conj(complexf(5, 7)) + complexf(3, 6) * std::conj(complexf(11, 13)));
  }

  void test_norm(void) {
    vector2c v;
    v[0] = complexf(1, -1);
    v[1] = complexf(1, -1);
    v[2] = complexf(1, -1);
    double expect;

    double n = norm(v, 2.0);
    expect   = sqrt(pow(abs(v[0]), 2) + pow(abs(v[1]), 2) + pow(abs(v[2]), 2));
    TS_ASSERT_DELTA(expect, n, expect * FLT_EPSILON);

    n      = norm(v, 3.0l);
    expect = pow(pow(abs(v[0]), 3) + pow(abs(v[1]), 3) + pow(abs(v[2]), 3), 1.0 / 3);
    TS_ASSERT_DELTA(expect, n, expect * FLT_EPSILON);
  }
};

#endif /* UUID_3A12F814_B92D_11E6_AB3C_0800274CD854 */
