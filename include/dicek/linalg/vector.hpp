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
#ifndef UUID_E2494444_AF31_11E6_9A65_0800274CD854
#define UUID_E2494444_AF31_11E6_9A65_0800274CD854

#include <array>

#include "../scalar_traits.hpp"
#include "../vector_base.hpp"

namespace dicek {
namespace math {
namespace linalg {
template<unsigned long _DIM, typename _scalar_type, typename _scalar_traits = scalar_traits<_scalar_type>>
class vector : public vector_base<vector<_DIM, _scalar_type, _scalar_traits>, _scalar_type, _scalar_traits> {
 public:
  static const unsigned long DIM = _DIM;
  typedef vector_base<vector<_DIM, _scalar_type, _scalar_traits>, _scalar_type, _scalar_traits> vector_base_;
  typedef typename vector_base_::scalar_traits scalar_traits;
  typedef typename vector_base_::scalar_type scalar_type;

  vector() : m_elm() {}

  scalar_type& at(unsigned long index) {
    return m_elm.at(index);
  }

  const scalar_type& at(unsigned long index) const {
    return m_elm.at(index);
  }

  scalar_type& operator[](unsigned long index) {
    return m_elm[index];
  }

  const scalar_type& operator[](unsigned long index) const {
    return m_elm[index];
  }

  template<typename F>
  vector map(F f) const {
    vector ret;

    for (unsigned long index = 0; index < DIM; ++index) {
      ret[index] = f((*this)[index]);
    }

    return ret;
  }

 private:
  std::array<scalar_type, DIM> m_elm;

  vector add_impl(const vector& rhs) const {
    vector ret;

    for (unsigned long index = 0; index < DIM; ++index) {
      ret[index] = (*this)[index] + rhs[index];
    }

    return ret;
  }

  vector scale_impl(scalar_type val) const {
    return map([val](scalar_type x) { return val * x; });
  }

  friend vector_base_;
};

template<typename V>
typename V::scalar_type inner_product(const V& lhs, const V& rhs) {
  typedef typename V::scalar_type scalar_type;
  scalar_type ret = scalar_type();

  for (unsigned long index = 0; index < V::DIM; ++index) {
    ret += lhs[index] * V::scalar_traits::conj(rhs[index]);
  }

  return ret;
}

template<typename V, typename scalar>
scalar norm(const V& v, scalar p) {
  using std::pow;
  if (p < scalar(1)) {
    throw std::range_error("p must be greater than or equal to 1");
  }

  typedef typename V::scalar_traits scalar_traits;

  scalar ret = 0;
  for (unsigned long index = 0; index < V::DIM; ++index) {
    ret += pow(scalar_traits::abs(v[index]), p);
  }

  return pow(ret, scalar(1) / p);
}
}  // namespace linalg
}  // namespace math
}  // namespace dicek

#endif /* UUID_E2494444_AF31_11E6_9A65_0800274CD854 */
