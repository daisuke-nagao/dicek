/*
MIT License

Copyright (c) 2022 Daisuke NAGAO

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: 
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
#ifndef UUID_6F484ACB_9C23_4013_A905_B5DAC701113A
#define UUID_6F484ACB_9C23_4013_A905_B5DAC701113A

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <dicek/scalar_traits.hpp>
#include <initializer_list>
#include <iterator>
#include <memory_resource>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace dicek::math::linalg {
template<typename T, typename scalar_traits = dicek::math::scalar_traits<T>>
class vector {
 public:
  using scalar_traits_type = scalar_traits;
  using scalar_type        = typename scalar_traits::scalar_type;

  template<typename pointer_type>
  class strided_iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = scalar_type;
    using difference_type   = std::ptrdiff_t;
    using pointer           = pointer_type;
    using reference         = std::conditional_t<std::is_const_v<std::remove_pointer_t<pointer_type>>, const value_type&, value_type&>;

    strided_iterator() : base_(nullptr), index_(0), step_(1) {}
    strided_iterator(pointer base, difference_type index, difference_type step) : base_(base), index_(index), step_(step) {}

    template<typename other_pointer_type, typename = std::enable_if_t<std::is_convertible_v<other_pointer_type, pointer_type>>>
    strided_iterator(const strided_iterator<other_pointer_type>& rhs) : base_(rhs.base_), index_(rhs.index_), step_(rhs.step_) {}

    reference operator*() const {
      return *(base_ + index_ * step_);
    }

    pointer operator->() const {
      return base_ + index_ * step_;
    }

    strided_iterator& operator++() {
      ++index_;
      return *this;
    }

    strided_iterator operator++(int) {
      strided_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    strided_iterator& operator--() {
      --index_;
      return *this;
    }

    strided_iterator operator--(int) {
      strided_iterator tmp(*this);
      --*this;
      return tmp;
    }

    strided_iterator& operator+=(difference_type n) {
      index_ += n;
      return *this;
    }

    strided_iterator& operator-=(difference_type n) {
      index_ -= n;
      return *this;
    }

    strided_iterator operator+(difference_type n) const {
      strided_iterator tmp(*this);
      tmp += n;
      return tmp;
    }

    friend strided_iterator operator+(difference_type n, strided_iterator rhs) {
      rhs += n;
      return rhs;
    }

    strided_iterator operator-(difference_type n) const {
      strided_iterator tmp(*this);
      tmp -= n;
      return tmp;
    }

    template<typename other_pointer_type>
    difference_type operator-(const strided_iterator<other_pointer_type>& rhs) const {
      return index_ - rhs.index_;
    }

    reference operator[](difference_type n) const {
      return *(*this + n);
    }

    template<typename other_pointer_type>
    bool operator==(const strided_iterator<other_pointer_type>& rhs) const {
      return base_ == rhs.base_ && index_ == rhs.index_ && step_ == rhs.step_;
    }

    template<typename other_pointer_type>
    bool operator!=(const strided_iterator<other_pointer_type>& rhs) const {
      return !(*this == rhs);
    }

    template<typename other_pointer_type>
    bool operator<(const strided_iterator<other_pointer_type>& rhs) const {
      return index_ < rhs.index_;
    }

    template<typename other_pointer_type>
    bool operator<=(const strided_iterator<other_pointer_type>& rhs) const {
      return index_ <= rhs.index_;
    }

    template<typename other_pointer_type>
    bool operator>(const strided_iterator<other_pointer_type>& rhs) const {
      return index_ > rhs.index_;
    }

    template<typename other_pointer_type>
    bool operator>=(const strided_iterator<other_pointer_type>& rhs) const {
      return index_ >= rhs.index_;
    }

   private:
    template<typename>
    friend class strided_iterator;

    pointer base_;
    difference_type index_;
    difference_type step_;
  };

  /* constructor (1) */
  vector() : length_(0), allocator_(), ref_count_(nullptr), elm_(nullptr), step_(1) {};
  /* constructor (2) */
  vector(std::size_t length, std::pmr::memory_resource* alloc = std::pmr::get_default_resource()) : length_(length), allocator_(alloc), ref_count_(nullptr), elm_(nullptr), step_(1) {
    using scalar_type_allocator_type                 = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<scalar_type>;
    using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
    scalar_type_allocator_type scalar_type_allocator = allocator_;
    elm_                                             = scalar_type_allocator_traits::allocate(scalar_type_allocator, length_);
    for (std::size_t i = 0; i < length_; ++i) {
      scalar_type_allocator_traits::construct(scalar_type_allocator, elm_ + i);
    }

    using size_t_allocator_type            = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<size_t>;
    using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
    size_t_allocator_type size_t_allocator = allocator_;
    ref_count_                             = size_t_allocator_traits::allocate(size_t_allocator, 1);
    size_t_allocator_traits::construct(size_t_allocator, ref_count_);
    ++*ref_count_;
  }
  /* constructor (3) */
  vector(scalar_type* buf, std::size_t length, int step = 1) : length_(length), allocator_(std::pmr::null_memory_resource()), ref_count_(nullptr), elm_(buf), step_(step) {
    if (step == 0) {
      throw std::invalid_argument("vector::vector: step must not be zero");
    }
  }
  /* constructor (4) */
  vector(std::initializer_list<scalar_type> ini, std::pmr::memory_resource* alloc = std::pmr::get_default_resource()) : vector(ini.size(), alloc) {
    std::copy(std::begin(ini), std::end(ini), std::begin(*this));
  }
  /* copy constructor */
  vector(const vector& rhs) : length_(rhs.length_), allocator_(rhs.allocator_), ref_count_(rhs.ref_count_), elm_(rhs.elm_), step_(rhs.step_) {
    if (ref_count_ != nullptr) {
      ++*ref_count_;
    }
  }
  /* move constructor */
  vector(vector&& rhs) noexcept
      : length_(std::exchange(rhs.length_, 0)), allocator_(std::exchange(rhs.allocator_, nullptr)), ref_count_(std::exchange(rhs.ref_count_, nullptr)), elm_(std::exchange(rhs.elm_, nullptr)), step_(std::exchange(rhs.step_, 1)) {}

  /* destructor */
  ~vector() noexcept {
    bool need_free = true;
    if (ref_count_ != nullptr) {
      --*ref_count_;
      if (*ref_count_ != 0) {
        need_free = false;
      }
    }
    if (need_free && ref_count_ != nullptr && allocator_ != nullptr) {
      using scalar_type_allocator_type                 = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<scalar_type>;
      using scalar_type_allocator_traits               = std::allocator_traits<scalar_type_allocator_type>;
      scalar_type_allocator_type scalar_type_allocator = allocator_;
      if (elm_ != nullptr) {
        for (std::size_t i = 0; i < length_; ++i) {
          scalar_type_allocator_traits::destroy(scalar_type_allocator, elm_ + i);
        }
        scalar_type_allocator_traits::deallocate(scalar_type_allocator, elm_, length_);
      }

      using size_t_allocator_type            = typename std::allocator_traits<std::pmr::polymorphic_allocator<std::byte>>::template rebind_alloc<size_t>;
      using size_t_allocator_traits          = std::allocator_traits<size_t_allocator_type>;
      size_t_allocator_type size_t_allocator = allocator_;
      if (ref_count_ != nullptr) {
        size_t_allocator_traits::destroy(size_t_allocator, ref_count_);
        size_t_allocator_traits::deallocate(size_t_allocator, ref_count_, 1);
      }
    }
  }

  vector& operator=(const vector& rhs) {
    if (this != &rhs) {
      vector(rhs).swap(*this);
    }
    return *this;
  }

  vector& operator=(vector&& rhs) noexcept {
    if (this != &rhs) {
      vector(std::move(rhs)).swap(*this);
    }
    return *this;
  }

  void swap(vector& rhs) noexcept {
    using std::swap;
    swap(length_, rhs.length_);
    swap(allocator_, rhs.allocator_);
    swap(ref_count_, rhs.ref_count_);
    swap(elm_, rhs.elm_);
    swap(step_, rhs.step_);
  }

  friend void swap(vector& lhs, vector& rhs) noexcept {
    lhs.swap(rhs);
  }

  using iterator       = strided_iterator<scalar_type*>;
  using const_iterator = strided_iterator<const scalar_type*>;

  const_iterator begin() const {
    return const_iterator(elm_, 0, step_);
  }
  const_iterator cbegin() const {
    return begin();
  }
  iterator begin() {
    return iterator(elm_, 0, step_);
  }
  const_iterator end() const {
    return const_iterator(elm_, static_cast<std::ptrdiff_t>(length_), step_);
  }
  const_iterator cend() const {
    return end();
  }
  iterator end() {
    return iterator(elm_, static_cast<std::ptrdiff_t>(length_), step_);
  }

  std::size_t size() const {
    return length_;
  }

  const scalar_type& operator[](std::size_t idx) const {
    return *(elm_ + static_cast<std::ptrdiff_t>(idx) * step_);
  }
  const scalar_type& at(size_t idx) const {
    if (idx >= length_) {
      throw std::out_of_range("vector::at: idx >= this->size()");
    }
    return operator[](idx);
  }

  scalar_type& operator[](std::size_t idx) {
    return const_cast<scalar_type&>(const_cast<const vector*>(this)->operator[](idx));
  }
  scalar_type& at(size_t idx) {
    return const_cast<scalar_type&>(const_cast<const vector*>(this)->at(idx));
  }

  const scalar_type* data() const {
    return elm_;
  }

  std::optional<std::size_t> ref_count() const {
    if (ref_count_ != nullptr) {
      return *ref_count_;
    } else {
      return std::nullopt;
    }
  }

  scalar_type* data() {
    return elm_;
  }

  vector clone(std::pmr::memory_resource* allocator) const {
    vector r(size(), allocator);
    std::copy(this->begin(), this->end(), r.begin());
    return r;
  }

  vector clone() const {
    return clone(get_allocator());
  }

  std::pmr::memory_resource* get_allocator() const noexcept {
    return allocator_;
  }

  template<typename F>
  vector map(F f) const {
    vector r(size(), result_allocator());
    std::transform(begin(), end(), r.begin(), f);
    return r;
  }

  vector add(const vector& rhs) const {
    validate_same_size(rhs, "vector::add");
    vector r(size(), result_allocator());
    for (std::size_t i = 0; i < size(); ++i) {
      r[i] = (*this)[i] + rhs[i];
    }
    return r;
  }

  vector subtract(const vector& rhs) const {
    validate_same_size(rhs, "vector::subtract");
    vector r(size(), result_allocator());
    for (std::size_t i = 0; i < size(); ++i) {
      r[i] = (*this)[i] - rhs[i];
    }
    return r;
  }

  vector scale(scalar_type val) const {
    return map([val](scalar_type x) { return x * val; });
  }

  vector operator+(const vector& rhs) const {
    return add(rhs);
  }

  vector operator-(const vector& rhs) const {
    return subtract(rhs);
  }

  vector operator-() const {
    return map([](scalar_type x) { return -x; });
  }

  vector operator*(scalar_type val) const {
    return scale(val);
  }

  vector operator/(scalar_type val) const {
    return map([val](scalar_type x) { return x / val; });
  }

  vector& operator+=(const vector& rhs) {
    validate_same_size(rhs, "vector::operator+=");
    for (std::size_t i = 0; i < size(); ++i) {
      (*this)[i] += rhs[i];
    }
    return *this;
  }

  vector& operator-=(const vector& rhs) {
    validate_same_size(rhs, "vector::operator-=");
    for (std::size_t i = 0; i < size(); ++i) {
      (*this)[i] -= rhs[i];
    }
    return *this;
  }

  vector& operator*=(scalar_type val) {
    for (auto& elm : *this) {
      elm *= val;
    }
    return *this;
  }

  vector& operator/=(scalar_type val) {
    for (auto& elm : *this) {
      elm /= val;
    }
    return *this;
  }

  friend vector operator*(scalar_type lhs, const vector& rhs) {
    return rhs.scale(lhs);
  }

 private:
  void validate_same_size(const vector& rhs, const char* name) const {
    if (size() != rhs.size()) {
      throw std::invalid_argument(std::string(name) + ": size mismatch");
    }
  }

  std::pmr::memory_resource* result_allocator() const noexcept {
    if (allocator_ == nullptr || allocator_ == std::pmr::null_memory_resource()) {
      return std::pmr::get_default_resource();
    }
    return allocator_;
  }

  std::size_t length_;
  std::pmr::memory_resource* allocator_;
  std::size_t* ref_count_;
  scalar_type* elm_;
  std::ptrdiff_t step_;
};

template<typename T, typename scalar_traits>
typename vector<T, scalar_traits>::scalar_type dot(const vector<T, scalar_traits>& lhs, const vector<T, scalar_traits>& rhs) {
  if (lhs.size() != rhs.size()) {
    throw std::invalid_argument("dot: size mismatch");
  }

  typename vector<T, scalar_traits>::scalar_type ret = {};
  for (std::size_t i = 0; i < lhs.size(); ++i) {
    ret += lhs[i] * scalar_traits::conj(rhs[i]);
  }
  return ret;
}

template<typename T, typename scalar_traits>
typename vector<T, scalar_traits>::scalar_type inner_product(const vector<T, scalar_traits>& lhs, const vector<T, scalar_traits>& rhs) {
  return dot(lhs, rhs);
}

template<typename T, typename scalar_traits, typename scalar>
auto norm(const vector<T, scalar_traits>& v, scalar p) {
  if (p < scalar(1)) {
    throw std::range_error("p must be greater than or equal to 1");
  }

  using return_type = decltype(std::pow(scalar_traits::abs(typename vector<T, scalar_traits>::scalar_type{}), p));
  return_type ret   = {};
  for (std::size_t i = 0; i < v.size(); ++i) {
    ret += std::pow(scalar_traits::abs(v[i]), p);
  }

  return std::pow(ret, return_type(1) / static_cast<return_type>(p));
}
}  // namespace dicek::math::linalg

#endif /* UUID_6F484ACB_9C23_4013_A905_B5DAC701113A */
